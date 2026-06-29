
#include "CrossLaserbeam.h"

#include <ProjectNull/Actor/Projectile/Laserbeam/Laserbeam.h>

ACrossLaserbeam::ACrossLaserbeam()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	
}

void ACrossLaserbeam::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACrossLaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrossLaserbeam::SetLaserEnabled(bool bEnabled)
{
	const ECollisionEnabled::Type CollisionType =
		bEnabled
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision;

	for (auto& Laserbeam : Laserbeams)
	{
		if (!Laserbeam) { continue; }

		Laserbeam->SetCollisionEnabled(CollisionType);
	}
}

