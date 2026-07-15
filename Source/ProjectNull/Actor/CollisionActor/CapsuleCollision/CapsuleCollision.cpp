#include "CapsuleCollision.h"

#include "Components\CapsuleComponent.h"

// Sets default values
ACapsuleCollision::ACapsuleCollision():
	CapsuleComponent(nullptr)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	if (!RootComponent) { return; }

	// カプセルの初期化(生成・デリゲート関数をセットetc)
	{
		CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
		if (!CapsuleComponent) { return; }
		CapsuleComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CapsuleComponent->SetupAttachment(RootComponent);
	}
}

// Called when the game starts or when spawned
void ACapsuleCollision::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACapsuleCollision::SetCollisionResponseToChannnel(const ECollisionChannel InCollisionChannel, const ECollisionResponse InCollisionResponse)
{
	if (!CapsuleComponent) { return; }
	CapsuleComponent->SetCollisionResponseToChannel(
		InCollisionChannel,
		InCollisionResponse
	);
}

void ACapsuleCollision::SetCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (!CapsuleComponent) { return; }

	CapsuleComponent->SetCollisionEnabled(InEnabled);
}

void ACapsuleCollision::SetRelativeTransform(const FTransform& InOffsetTransform)
{
	if (!CapsuleComponent) { return; }

	CapsuleComponent->SetRelativeTransform(InOffsetTransform);
}