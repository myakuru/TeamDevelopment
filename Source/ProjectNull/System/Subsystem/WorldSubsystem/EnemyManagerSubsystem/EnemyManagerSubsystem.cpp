
#include "EnemyManagerSubsystem.h"

#include "Kismet/GameplayStatics.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManagerConfig.h>
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
	auto* Config = Cast<UEnemyISMManagerConfig>(ISMManagerPath.TryLoad());

	if (!Config)
	{
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManagerSubsystem] Failed to load ISMManagerConfig"));
		return;
	}

	for (const TSubclassOf<AEnemyISMManager>& ManagerClass : Config->ISMManagerClasses)
	{
		if (!ManagerClass) { continue; }
		GetWorld()->SpawnActor<AEnemyISMManager>(ManagerClass, FTransform::Identity);
	}
}

void UEnemyManagerSubsystem::RegisterEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }

	EnemyGruntList.Add(Enemy);
}

void UEnemyManagerSubsystem::RemoveEnemy(AEnemyBase* Enemy)
{
	if (!Enemy) { return; }

	EnemyGruntList.Remove(Enemy);
}

void UEnemyManagerSubsystem::UpdateEnemies(float DeltaTime)
{
	// プレイヤーの情報を取得する（0番:1P）
	APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PPlayerPawn) { return; }
	
	// すべて敵の更新メソッドを呼ぶ
	for (AEnemyBase* Enemy : EnemyGruntList) 
	{
		if (Enemy) 
		{
			Enemy->OnUpdate(PPlayerPawn, DeltaTime);
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
		UE_LOG(LogTemp, Warning, TEXT("[EnemyManagerSubsystem] RegisterISMManager : Duplicate Manager Class : %s"), *ManagerClass->GetName());
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
