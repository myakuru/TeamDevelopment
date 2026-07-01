#include "Laserbeam.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Effect/EffectBase.h>


ALaserbeam::ALaserbeam()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

}

void ALaserbeam::BeginPlay()
{
	Super::BeginPlay();

}

void ALaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserbeam::SetCollisionEnabled(const ECollisionEnabled::Type CollisionType)
{
	
}
