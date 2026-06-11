#include "CapsuleCollision.h"

// Sets default values
ACapsuleCollision::ACapsuleCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACapsuleCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACapsuleCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

