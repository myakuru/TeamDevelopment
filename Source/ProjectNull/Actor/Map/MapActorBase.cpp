#include "MapActorBase.h"

#include "Kismet/GameplayStatics.h"

AMapActorBase::AMapActorBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AMapActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMapActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool AMapActorBase::OnHitDistancePlayer()
{
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (playerPawn == nullptr) { return false; }

	float distance = FVector::Dist(
		GetActorLocation(),
		playerPawn->GetActorLocation()
	);

	return distance < ActorParams.HitDistance;
}

