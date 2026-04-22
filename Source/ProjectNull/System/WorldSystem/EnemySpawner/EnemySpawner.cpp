
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
	if (CachedSubsystem)
	{
		CachedSubsystem->OnPhaseChanged.RemoveAll(this);
	}
	Super::EndPlay(EndPlayReason);
}

void AEnemySpawner::SpawnEnemy()
{
	// 出現対象が存在しなかったら実行しない
	if (!EnemyClass) { return; }

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

	for (const FEnemySpawnUnit& Unit : CurrentWaveData->Enemies)
	{
		if (!Unit.EnemyClass)
		{
			UE_LOG(LogTemp, Error, TEXT("EnemyClass is null"));
			continue;
		}
		if (!Unit.SpawnPattern)
		{
			UE_LOG(LogTemp, Error, TEXT("SpawnPattern is null"));
			continue;
		}

		// 出現座標を決める
		TArray<FVector> spawnLocations =
			Unit.SpawnPattern->GenerateSpawnTransforms(Unit.SpawnCount, playerLocation);

		UE_LOG(LogTemp, Warning, TEXT("Transforms count: %d"), spawnLocations.Num());

		// Count分エネミーを生成
		for (FVector& spawnLocation : spawnLocations)
		{
			// 当たり判定の結果
			FHitResult hitResult;

			// Rayが静的なオブジェクトに衝突していて
			// 衝突している場所が地面または坂の傾きの場合は（壁などでない場合）
			// 敵を出現させる
			if (IsIntersectingStaticObjects(hitResult, spawnLocation)
				&& hitResult.Normal.Z > SpawnParams.MinGroundNormalZ) {
				// 情報に基づいてアクターを出現させる
				AActor* SpawnedEnemy = GetWorld()->SpawnActor<AActor>(
					Unit.EnemyClass,
					spawnLocation,
					FRotator::ZeroRotator);

				UE_LOG(LogTemp, Warning, TEXT("SpawnActor result: %s"),
					SpawnedEnemy ? TEXT("SUCCESS") : TEXT("FAILED"));
			}
		}
	}
}

FVector AEnemySpawner::CalculateEnemySpawnPointInRing(const FVector& Center) const
{
	// ランダム角度
	float angle = FMath::RandRange(0.0f, kFullCircleDeg);

	// ランダム半径
	const float distance = FMath::FRandRange(SpawnParams.SpawnMinRadius, SpawnParams.SpawnMaxRadius);

	// XYオフセット
	FVector offset = {
		FMath::Cos(FMath::DegreesToRadians(angle)) * distance,
		FMath::Sin(FMath::DegreesToRadians(angle)) * distance,
		0.0f
	};

	return Center + offset;
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

	UEnemyWaveDataAsset* NewWaveData = 
		const_cast<UEnemyWaveDataAsset*>(PhaseSpawnTable->FindWaveDataByPhase(NewPhase));
	
	if (!NewWaveData)
	{
		UE_LOG(LogTemp, Error, TEXT("No WaveData found Phase : %d"), NewPhase);
		CurrentWaveData = nullptr;
		return;
	}

	CurrentWaveData = NewWaveData;
}

//
//void AEnemySpawner::Tick(float DeltaTime)
//{
//	// プレイヤーの情報を取得する（0番:1P）
//	const APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//	if (!pPlayerPawn) { return; }
//
//	// プレイヤーの場所（Location）
//	const FVector playerLocation = pPlayerPawn->GetActorLocation();
//
//	// 出現座標XY
//	FVector spawnLocation = CalculateEnemySpawnPointInRing(playerLocation);
//
//	// Rayの座標を求める
//	FVector rayStart = spawnLocation + FVector(0.0f, 0.0f, SpawnParams.RayStartHeight);
//	FVector rayEnd = spawnLocation - FVector(0.0f, 0.0f, SpawnParams.RayEndDepth);
//
//	Super::Tick(DeltaTime);
//	DrawDebugLine(GetWorld(), rayStart, rayEnd, FColor::Green, false, 0.1f, 0, 2);
//
//}

