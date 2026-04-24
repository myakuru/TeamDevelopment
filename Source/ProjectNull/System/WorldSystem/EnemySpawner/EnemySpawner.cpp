
#include "EnemySpawner.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

#include "EnemySpawnPattern/EnemySpawnPatternBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyGrunt/EnemyGruntBase.h>

#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/System/WorldSystem/EnemySpawner/EnemyPhaseSpawnTable.h>
#include "Engine/GameInstance.h"

AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

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
	}
}

void AEnemySpawner::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// 登録していたフェーズ情報を解除
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

	UE_LOG(LogTemp, Warning, TEXT("SpawnWave called"));

	if (!CurrentWaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("WaveData is null"));
		return;
	}

	// プレイヤーの情報を取得する（0番:1P）
	const APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	// プレイヤーの場所（Location）
	const FVector playerLocation = pPlayerPawn->GetActorLocation();

	// フェーズに対応した生成数を取得
	const int32 spawnNum = PhaseSpawnTable->FindEnemyNumByPhase(NowPhase);

	// 各データテーブルの持つフェーズの敵生成数に応じて敵を生成
	for (int i = 0; i < spawnNum; i++)
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
					Unit.SpawnPattern->GenerateSpawnTransforms(1, playerLocation);

				for (FVector& SpawnLocation : SpawnLocations)
				{
					FHitResult HitResult;
					if (IsIntersectingStaticObjects(HitResult, SpawnLocation)
						&& HitResult.Normal.Z > SpawnParams.MinGroundNormalZ)
						// 情報に基づいてアクターを出現させる
					{
						GetWorld()->SpawnActor<AActor>(
							Unit.EnemyClass, SpawnLocation, FRotator::ZeroRotator);
					}
				}
				break;
			}
		}
	}
}

bool AEnemySpawner::IsIntersectingStaticObjects(FHitResult& HitResult, FVector& SpawnLocationXY)
{
	// Rayの座標を求める
	FVector rayStart = SpawnLocationXY + FVector(0.0f, 0.0f, SpawnParams.RayStartHeight);
	FVector rayEnd = SpawnLocationXY - FVector(0.0f, 0.0f, SpawnParams.RayEndDepth);

	// Rayがワールドの静的オブジェクトに衝突しているか調べる
	const bool isIntersect = GetWorld()->LineTraceSingleByChannel(HitResult, rayStart, rayEnd, ECollisionChannel::ECC_Visibility);

	// 衝突していたら衝突した座標を出現座標にする
	if (isIntersect)
	{
		SpawnLocationXY = HitResult.Location;
		SpawnLocationXY.Z += SpawnParams.SpawnOffsetZ;
	}

	return isIntersect;
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
		CurrentWaveData = nullptr;
		return;
	}

	CurrentWaveData = NewWaveData;
	NowPhase = NewPhase;
}
