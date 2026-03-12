// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void AEnemySpawner::SpawnEnemy()
{
	//　出現対象が存在しなかったら実行しない
	if (!EnemyClass) { return; }

	//　プレイヤーの情報を取得する（0番:1P）
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	//　プレイヤーの場所（Location）
	const FVector playerLocation = pPlayerPawn->GetActorLocation();

	//　ランダム半径
	const float randomRadius = FMath::FRandRange(SpawnMinRadius, SpawnMaxRadius);

	//　出現半径に基づいたランダムなベクトル
	const FVector randomOffset = FMath::VRand() * randomRadius;

	//　出現座標
	const FVector spawnLocation = playerLocation + randomOffset;

	//　情報に基づいてアクターを出現させる
	GetWorld()->SpawnActor<AActor>(
		EnemyClass,
		spawnLocation,
		FRotator::ZeroRotator);
}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorld()->GetTimerManager().SetTimer(
		SpawnTimerHandle,
		this,
		&AEnemySpawner::SpawnEnemy,
		SpawnInterval,
		true);

}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

