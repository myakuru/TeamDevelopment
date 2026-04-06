
#include "EnemySpawner.h"

#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectNull/Actor/Character/Enemy/EnemyGrunt/EnemyGruntBase.h"


AEnemySpawner::AEnemySpawner()
{
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	//　出現タイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		SpawnParams.SpawnInterval,
		true);

}

void AEnemySpawner::SpawnEnemy()
{
	//　出現対象が存在しなかったら実行しない
	if (!EnemyClass) { return; }

	//　プレイヤーの情報を取得する（0番:1P）
	const APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	//　プレイヤーの場所（Location）
	const FVector playerLocation = pPlayerPawn->GetActorLocation();

	//　出現座標XY
	FVector spawnLocation = CalculateEnemySpawnPointInRing(playerLocation);

	//　当たり判定の結果
	FHitResult hitResult;

	//　Rayが静的なオブジェクトに衝突していて
	//	衝突している場所が地面または坂の傾きの場合は（壁などでない場合）
	//　敵を出現させる
	if (IsIntersectingStaticObjects(hitResult, spawnLocation)
		&& hitResult.Normal.Z > SpawnParams.MinGroundNormalZ) {
		//　情報に基づいてアクターを出現させる
		AEnemyGruntBase* enemyGrunt = GetWorld()->SpawnActor<AEnemyGruntBase>(
			EnemyClass,
			spawnLocation,
			FRotator::ZeroRotator);
	}
}

FVector AEnemySpawner::CalculateEnemySpawnPointInRing(const FVector& Center) const
{
	//　ランダム角度
	float angle = FMath::RandRange(0.0f, kFullCircleDeg);

	//　ランダム半径
	const float distance = FMath::FRandRange(SpawnParams.SpawnMinRadius, SpawnParams.SpawnMaxRadius);

	//　XYオフセット
	FVector offset = {
		FMath::Cos(FMath::DegreesToRadians(angle)) * distance,
		FMath::Sin(FMath::DegreesToRadians(angle)) * distance,
		0.0f
	};

	return Center + offset;
}

bool AEnemySpawner::IsIntersectingStaticObjects(FHitResult& HitResult, FVector& SpawnLocationXY)
{
	//　Rayの座標を求める
	FVector rayStart = SpawnLocationXY + FVector(0.0f, 0.0f, SpawnParams.RayStartHeight);
	FVector rayEnd = SpawnLocationXY - FVector(0.0f, 0.0f, SpawnParams.RayEndDepth);

	//　Rayがワールドの静的オブジェクトに衝突しているか調べる
	const bool isIntersect = GetWorld()->LineTraceSingleByChannel(HitResult, rayStart, rayEnd, ECollisionChannel::ECC_Visibility);

	//　衝突していたら衝突した座標を出現座標にする
	if (isIntersect)
	{
		SpawnLocationXY = HitResult.Location;
		SpawnLocationXY.Z += SpawnParams.SpawnOffsetZ;
	}

	return isIntersect;
}

//
//void AEnemySpawner::Tick(float DeltaTime)
//{
//	//　プレイヤーの情報を取得する（0番:1P）
//	const APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
//	if (!pPlayerPawn) { return; }
//
//	//　プレイヤーの場所（Location）
//	const FVector playerLocation = pPlayerPawn->GetActorLocation();
//
//	//　出現座標XY
//	FVector spawnLocation = CalculateEnemySpawnPointInRing(playerLocation);
//
//	//　Rayの座標を求める
//	FVector rayStart = spawnLocation + FVector(0.0f, 0.0f, SpawnParams.RayStartHeight);
//	FVector rayEnd = spawnLocation - FVector(0.0f, 0.0f, SpawnParams.RayEndDepth);
//
//	Super::Tick(DeltaTime);
//	DrawDebugLine(GetWorld(), rayStart, rayEnd, FColor::Green, false, 0.1f, 0, 2);
//
//}

