
#include "EnemySpawner.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "EnemySpawnPattern/EnemySpawnPatternBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyGrunt/EnemyGruntBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>

#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/System/WorldSystem/EnemySpawner/EnemyPhaseSpawnTable.h>
#include <ProjectNull/System/WorldSystem/EnemyPoolSubSystem/EnemyPoolSubSystem.h>
#include "Engine/GameInstance.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::SetFinalPhase()
{
	if (FinalPhase) { return; }
	FinalPhase = true;

	ApplySpawnModeByPhase(PhaseSpawnTable->FinalWave);
}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// フェーズテーブル情報が存在する時のみ実行
	if (PhaseSpawnTable)
	{
		UEnemyPoolSubSystem* PoolSubSystem =
			GetWorld()->GetSubsystem<UEnemyPoolSubSystem>();

		for (const FPhaseSpawnWave& WaveData : PhaseSpawnTable->PhaseWaves)
		{
			if (!WaveData.WaveData) { continue; }

			// ウェーブデータ分のプールを確保
			for (const FEnemySpawnUnit& SpawnUnit : WaveData.WaveData->Enemies)
			{
				if (SpawnUnit.PoolConfig && PoolSubSystem)
				{
					// 生成するデータのDataAssetを入れる
					PoolSubSystem->WarmUp(SpawnUnit.PoolConfig);
				}
			}
		}
	}

	// 出現タイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		SpawnParams.SpawnInterval,
		true);

	// フェーズ情報を取得
	CachedSubsystem = GetWorld()->GetSubsystem<UGameProgressSubsystem>();
	if (CachedSubsystem)
	{
		CachedSubsystem->OnPhaseChanged.AddUObject(this, &AEnemySpawner::HandlePhaseChanged);
		HandlePhaseChanged(CachedSubsystem->GetPhase());
		
		CachedSubsystem = GetWorld()->GetSubsystem<UGameProgressSubsystem>();
		if (CachedSubsystem)
		{
			CachedSubsystem->SetPhaseThresholds(PhaseSpawnTable->PhaseWaves[0].PhaseUpDeathEnemyCount);
		}
	}
}

void AEnemySpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 登録していたフェーズ情報を解除
	// レベル遷移時にダングリングポインタが発生する
	if (CachedSubsystem)
	{
		CachedSubsystem->OnPhaseChanged.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AEnemySpawner::SpawnEnemy()
{
	// 出現対象が存在しなかったら実行しない
	//if (!EnemyClass) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("SpawnWave called"));

	if (!CurrentWaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveData is null"));
		return;
	}

	// プレイヤーの情報を取得する（0番:1P）
	const APawn* PPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PPlayerPawn) { return; }

	// プレイヤーの場所（Location）
	const FVector PlayerLocation = PPlayerPawn->GetActorLocation();

	// フェーズに対応した生成数を取得
	const int32 SpawnNum = PhaseSpawnTable->FindEnemyNumByPhase(NowPhase);

	// プールのサブシステムを取得
	UEnemyPoolSubSystem* poolSubSystem = GetWorld()->GetSubsystem<UEnemyPoolSubSystem>();

	// 各データテーブルの持つフェーズの敵生成数に応じて敵を生成
	for (int i = 0; i < SpawnNum; i++)
	{
		// 確率の合計を計算
		int TotalWeight = 0;
		for (const FEnemySpawnUnit& Unit : CurrentWaveData->Enemies)
		{
			TotalWeight += Unit.CreateProbability;
		}

		// 合計値の範囲でランダムを取得
		const int Roll = FMath::RandRange(0, TotalWeight - 1);
		int Accumulated = 0;

		for (const FEnemySpawnUnit Unit : CurrentWaveData->Enemies)
		{
			Accumulated += Unit.CreateProbability;

			// 累積確率を超えたらこの敵を選択
			// 乱数よりも敵生成確率が低ければ次の敵に移って再生成
			// 累積確率を超えたらこの敵を選択
			if (Roll < Accumulated)
			{
				if (!Unit.EnemyClass || !Unit.SpawnPattern) { break; }

				// 座標を出現パターンの派生先から取得
				TArray<FVector> SpawnLocations =
					Unit.SpawnPattern->GenerateSpawnTransforms(1, PlayerLocation);

				for (FVector& SpawnLocation : SpawnLocations)
				{
					FHitResult HitResult;
					//if (IsIntersectingStaticObjects(HitResult, SpawnLocation)
					//&& HitResult.Normal.Z > SpawnParams.MinGroundNormalZ)
					if (!IsIntersectingStaticObjects(HitResult, SpawnLocation)) { continue; }
					if (HitResult.Normal.Z <= SpawnParams.MinGroundNormalZ) { continue; }
					// 情報に基づいてアクターを出現させる
					{
						// プール経由でスポーンする
						if (Unit.PoolConfig && poolSubSystem)
						{
							if (poolSubSystem->IsPoolFull(Unit.PoolConfig))
							{
								//UE_LOG(LogTemp, Warning, TEXT("[Spawner] Pool is full. Skip spawn: %s"), *Unit.PoolConfig->GetName());
								continue;
							}

							poolSubSystem->Spawn(Unit.PoolConfig, SpawnLocation);
						}
					}
					break;
				}
				break;
			}
		}
	}
}

bool AEnemySpawner::IsIntersectingStaticObjects(FHitResult& HitResult, FVector& SpawnLocationXY)
{
	// Rayの座標を求める
	FVector RayStart = SpawnLocationXY + FVector(0.0f, 0.0f, SpawnParams.RayStartHeight);
	FVector RayEnd = SpawnLocationXY - FVector(0.0f, 0.0f, SpawnParams.RayEndDepth);

	// Rayがワールドの静的オブジェクトに衝突しているか調べる
	const bool IsIntersect = GetWorld()->LineTraceSingleByChannel(HitResult, RayStart, RayEnd, ECollisionChannel::ECC_Visibility);

	// 衝突していたら衝突した座標を出現座標にする
	if (IsIntersect)
	{
		SpawnLocationXY = HitResult.Location;
		SpawnLocationXY.Z += SpawnParams.SpawnOffsetZ;
	}

	return IsIntersect;
}

void AEnemySpawner::HandlePhaseChanged(int NewPhase)
{
	ApplySpawnModeByPhase(NewPhase);
}

void AEnemySpawner::ApplySpawnModeByPhase(int NewPhase)
{
	if (!PhaseSpawnTable)
	{
		UE_LOG(LogTemp, Error, TEXT("PhaseSpawnTable is null"));
		CurrentWaveData = nullptr;
		return;
	}

	// 新しいウェーブデータを取得
	UEnemyWaveDataAsset* NewWaveData = 
		const_cast<UEnemyWaveDataAsset*>(PhaseSpawnTable->FindWaveDataByPhase(NewPhase));

	if (!NewWaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("No WaveData found Phase : %d"), NewPhase);
		//CurrentWaveData = nullptr;
		return;
	}

	CurrentWaveData = NewWaveData;
	NowPhase = NewPhase;

	CachedSubsystem = GetWorld()->GetSubsystem<UGameProgressSubsystem>();
	if (CachedSubsystem)
	{
		CachedSubsystem->SetPhaseThresholds(PhaseSpawnTable->PhaseWaves[NewPhase].PhaseUpDeathEnemyCount);
	}

	// 中ボス出現
	if (PhaseSpawnTable->FinalWave == NewPhase)
	{
		FinalPhase = true;

		if (PhaseSpawnTable->StageBoss)
		{

			AEnemyBossBase* Boss = GetWorld()->SpawnActor<AEnemyBossBase>(
				PhaseSpawnTable->StageBoss,
				GetActorLocation(),
				GetActorRotation());

			if (!IsValid(Boss))
			{
				return;
			}
		}
	}

	// ボス出現
	if (PhaseSpawnTable->BossWave == NewPhase)
	{
		BossPhase = true;

		if (PhaseSpawnTable->StageBoss)
		{

			AEnemyBossBase* Boss = GetWorld()->SpawnActor<AEnemyBossBase>(
				PhaseSpawnTable->StageBoss,
				GetActorLocation(),
				GetActorRotation());

			if (!IsValid(Boss))
			{
				return;
			}
		}
	}
}
