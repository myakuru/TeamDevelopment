#include "BoxCollision.h"

#include "Components\BoxComponent.h"

// Sets default values
ABoxCollision::ABoxCollision():
	BoxComponent(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }

	// 球の初期化(生成・デリゲート関数をセットetc)
	{
		BoxComponent = CreateDefaultSubobject<UBoxComponent>("Box");
		if (!BoxComponent) { return; }
		BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		BoxComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ABoxCollision::BeginPlay()
{
	Super::BeginPlay();
}

void ABoxCollision::SetCollisionResponseToChannnel(const ECollisionChannel InCollisionChannel, const ECollisionResponse InCollisionResponse)
{
	if (!BoxComponent) { return; }
	BoxComponent->SetCollisionResponseToChannel(
		InCollisionChannel,
		InCollisionResponse
	);
}

void ABoxCollision::SetCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (!BoxComponent) { return; }

	BoxComponent->SetCollisionEnabled(InEnabled);
}

void ABoxCollision::SetRelativeTransform(const FTransform& InOffsetTransform)
{
	if (!BoxComponent) { return; }

	BoxComponent->SetRelativeTransform(InOffsetTransform);
}