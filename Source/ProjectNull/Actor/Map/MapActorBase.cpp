#include "MapActorBase.h"

#include "Kismet/GameplayStatics.h"

AMapActorBase::AMapActorBase()
{
	PrimaryActorTick.bCanEverTick = false;

	Trigger = CreateDefaultSubobject<USphereComponent>(TEXT("Trigger"));
	RootComponent = Trigger;

	Trigger->InitSphereRadius(ActorParams.HitDistance);

	// 衝突設定
	Trigger->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Trigger->SetCollisionObjectType(ECC_WorldDynamic);
	Trigger->SetCollisionResponseToAllChannels(ECR_Ignore);
	Trigger->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	//// イベント登録
	//Trigger->OnComponentBeginOverlap.AddDynamic(
	//	this,
	//	&AMapActorBase::HitReaction
	//);
}

void AMapActorBase::BeginPlay()
{
	Super::BeginPlay();
}

void AMapActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMapActorBase::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Trigger)
	{
		Trigger->SetSphereRadius(ActorParams.HitDistance);
	}
}

float AMapActorBase::DistanceFromPlayer()
{
	//プレイヤー取得
	APawn* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (playerPawn == nullptr) { return false; }

	FVector distLocation = playerPawn->GetActorLocation() - GetActorLocation();

	//距離の二乗
	float distance = distLocation.X * distLocation.X + distLocation.Y * distLocation.Y + distLocation.Z * distLocation.Z;

	return distance < ActorParams.HitDistance * ActorParams.HitDistance;
}
