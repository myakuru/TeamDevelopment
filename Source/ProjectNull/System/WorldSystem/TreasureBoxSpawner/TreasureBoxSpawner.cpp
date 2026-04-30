#include "TreasureBoxSpawner.h"

ATreasureBoxSpawner::ATreasureBoxSpawner()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATreasureBoxSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void ATreasureBoxSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

