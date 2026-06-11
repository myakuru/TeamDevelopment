#include "SphereCollision.h"

#include "Components\SphereComponent.h"

// Sets default values
ASphereCollision::ASphereCollision()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// 球の初期化(生成・デリゲート関数をセットetc)
	{
		SphereComponent = CreateDefaultSubobject<USphereComponent>("Sphere");
		SphereComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ASphereCollision::BeginPlay()
{
	Super::BeginPlay();
}

void ASphereCollision::DetachRootComponent()
{
	RootComponent->DetachFromComponent(FDetachmentTransformRules::KeepRelativeTransform);
}