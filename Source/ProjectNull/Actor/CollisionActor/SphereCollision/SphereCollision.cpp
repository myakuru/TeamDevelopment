#include "SphereCollision.h"

#include "Components\SphereComponent.h"

// Sets default values
ASphereCollision::ASphereCollision()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }

	// 球の初期化(生成・デリゲート関数をセットetc)
	{
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
		if (!SphereComponent) { return; }
		SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ASphereCollision::BeginPlay()
{
	Super::BeginPlay();
}

void ASphereCollision::SetCollisionResponseToChannnel(const ECollisionChannel InCollisionChannel, const ECollisionResponse InCollisionResponse)
{
	if (!SphereComponent) { return; }
	SphereComponent->SetCollisionResponseToChannel(
		InCollisionChannel,
		InCollisionResponse
	);
}

void ASphereCollision::SetCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (!SphereComponent) { return; }

	SphereComponent->SetCollisionEnabled(InEnabled);
}

void ASphereCollision::SetRelativeTransform(const FTransform& InOffsetTransform)
{
	if (!SphereComponent) { return; }

	SphereComponent->SetRelativeTransform(InOffsetTransform);
}