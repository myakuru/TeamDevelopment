
#include "EnemyManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManagerConfig.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerConfig.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

void UEnemyManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	UWorldSubsystem::Initialize(Collection);
}

void UEnemyManagerSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	// データアセットのパスを直接指定して読む
	const FSoftObjectPath ISMManagerPath(TEXT("/Game/Actor/Manager/DA_EnemyISMManagerConfig.DA_EnemyISMManagerConfig"));
	ISMManagerConfig = Cast<UEnemyISMManagerConfig>(ISMManagerPath.TryLoad());

	if (!ISMManagerConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManagerSubsystem Failed to load ISMManagerConfig"));
		return;
	}

	// エネミーマネージャーのコンフィグデータ読み込み
	const FSoftObjectPath ManagerPath(TEXT("/Game/Actor/Manager/DA_EnemyManagerConfig.DA_EnemyManagerConfig"));
	ManagerConfig = Cast<UEnemyManagerConfig>(ManagerPath.TryLoad());

	if (!ManagerConfig)
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManagerSubsystem Failed to load ManagerConfig"));
		return;
	}

	for (const TSubclassOf<AEnemyISMManager>& ManagerClass : ISMManagerConfig->ISMManagerClasses)
	{
		if (!ManagerClass) { continue; }
		GetWorld()->SpawnActor<AEnemyISMManager>(ManagerClass, FTransform::Identity);
	}
}

void UEnemyManagerSubsystem::RegisterEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }
	if (EnemyGruntList.Contains(Enemy)) { return; }

	// 登録時にインターバルを設定
	APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (PPlayerPawn)
	{
		float Dist = CalcDistance(Enemy->GetActorLocation(), PPlayerPawn->GetActorLocation());
		Enemy->SetUpdateInterval(CalcInterval(Dist));
	}

	EnemyGruntList.Add(Enemy);
}

void UEnemyManagerSubsystem::RemoveEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }

	EnemyGruntList.Remove(Enemy);
}

void UEnemyManagerSubsystem::UpdateEnemies(float DeltaTime)
{
	FrameCount++;

	// 定期的にカウントリセット
	if (FrameCount >= 3600) { FrameCount = 0; }
	// プレイヤーの情報を取得する（0番:1P）
	APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PPlayerPawn) { return; }
	//// 距離に応じて敵のUpdate回数を変更
	//FVector PlayerPos = PPlayerPawn->GetActorLocation();
	
	// 敵の更新回数をスキップするための変数を求める（10フレームに一回チェック）
	if (FrameCount % 10 == 0)
	{
		for (AEnemyBase* Enemy : EnemyGruntList)
		{
			if (!Enemy) { continue; }
			if (!Enemy->GetAliveFlg()) { continue; }

			// 距離を求める
			//float dist = CalcDistance(Enemy->GetActorLocation(), PlayerPos);
			
			// Updateの回数を割る数を求める
			Enemy->SetUpdateInterval(CalcInterval(Enemy->GetTargetDistanceSqr()));
		}
	}

	// すべて敵の更新メソッドを呼ぶ
	for (AEnemyBase* Enemy : EnemyGruntList) 
	{
		if (Enemy) 
		{
			if (Enemy->GetAliveFlg())
			{
				int32 Interval = Enemy->GetUpdateInterval();
				if (FrameCount % Interval != 0) { continue; }
				Enemy->OnUpdate(PPlayerPawn, DeltaTime * Interval);
			}
		}
	}

	// ISMManagerの描画更新を呼ぶ
	for (auto& Pair : ISMManagerMap)
	{
		if (Pair.Value)
		{
			Pair.Value->UpdateEnemies(DeltaTime);
		}
	}
}

void UEnemyManagerSubsystem::RegisterISMManager(AEnemyISMManager* Manager)
{
	if (!Manager) { return; }

	UClass* ManagerClass = Manager->GetClass();

	// すでに同じクラスのManagerが登録されている場合はスキップする
	if (ISMManagerMap.Contains(ManagerClass))
	{
		UE_LOG(LogTemp, Warning, TEXT("EnemyManagerSubsystem RegisterISMManager : Duplicate Manager Class : %s"), *ManagerClass->GetName());
		return;
	}

	ISMManagerMap.Add(ManagerClass, Manager);
}

AEnemyISMManager* UEnemyManagerSubsystem::GetISMManager(TSubclassOf<AEnemyISMManager> ManagerClass) const
{
	if (!ManagerClass) { return nullptr; }

	// Getで生のUClass*と比較する
	UClass* Key = ManagerClass.Get();
	// TMapからクラスに対応するManagerを取得
	auto* Found = ISMManagerMap.Find(Key);
	if (Found)
	{
		return Found->Get();
	}
	else
	{
		return nullptr;
	}
}

float UEnemyManagerSubsystem::CalcDistance(const FVector& EnemyPos, const FVector& PlayerPos)
{
	return  FVector::DistSquared(EnemyPos, PlayerPos);
}

int32 UEnemyManagerSubsystem::CalcInterval(float Distance)
{
	// 最低でも１回はUpdateするため、１を代入する
	int32 Interval = 1;
	for (auto& EnemyData : ManagerConfig->UpdateLimitDistance)
	{
		// プレイヤーとの距離が一定以上ならUpdateの回数を減らす
		// Distanceを作るとき、DistSquaredを使っているので、閾値のほうも2乗する
		if (Distance > (EnemyData * EnemyData))
		{
			Interval *= 2;
		}
		else
		{
			break;
		}
	}
	return Interval;
}
