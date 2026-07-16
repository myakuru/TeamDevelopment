
#include "AutoAttackHitActor.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

AAutoAttackHitActor::AAutoAttackHitActor():
	HitActors(TSet<TObjectPtr<AActor>>()),
	PreviousLocation(FVector::ZeroVector),
	BoxComp(nullptr),
	GameInstance(nullptr),
	PlayerRuntimeData(nullptr),
	AttackPowerScale(1.f),
	bEnabled(false)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	if (!BoxComp) { return; }
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(
		ECollisionEnabled::Type::NoCollision);

}

void AAutoAttackHitActor::BeginPlay()
{
	Super::BeginPlay();

	// ゲームインスタンス取得
	GameInstance = Cast<USuperGameInstance>(GetWorld()->GetGameInstance());

	if (!GameInstance) { return; }
	PlayerRuntimeData = GameInstance->GetPlayerRuntimeData();

	if (!BoxComp) { return; }
	PreviousLocation = BoxComp->GetComponentLocation();
}

void AAutoAttackHitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	PerformHitSweep();

	if (!BoxComp) { return; }
	PreviousLocation = BoxComp->GetComponentLocation();
}

void AAutoAttackHitActor::SetHitEnabled(bool bInEnabled)
{
	const bool bWasEnabled = bEnabled;
	bEnabled = bInEnabled;

	// OFF -> ON の瞬間だけ、ヒット済みリストと基準位置をリセット
	if (bEnabled && !bWasEnabled)
	{
		HitActors.Empty();

		if (BoxComp)
		{
			PreviousLocation = BoxComp->GetComponentLocation();
		}
	}
}

void AAutoAttackHitActor::PerformHitSweep()
{
	if (!bEnabled) { return; }
	if (!BoxComp) { return; }
	//UE_LOG(LogTemp, Display, TEXT("hi 生成"));

	const FVector CurrentLocation =
		BoxComp->GetComponentLocation();

	const FVector BoxExtent =
		BoxComp->GetScaledBoxExtent();

	TArray<FHitResult> Hits;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	if (GetOwner())
	{
		Params.AddIgnoredActor(GetOwner());
	}

	const bool bHit =
		GetWorld()->SweepMultiByChannel(
			Hits,
			PreviousLocation,
			CurrentLocation,
			BoxComp->GetComponentQuat(),
			ECC_Enemy,
			FCollisionShape::MakeBox(BoxExtent),
			Params);

	if (!bHit) { return; }

	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!HitActor) { continue; }
		
		// 既に当たった敵は除外
		if (HitActors.Contains(HitActor)) { continue; }

		HitActors.Add(HitActor);

		auto* Interface = Cast<ICharacterInterface>(HitActor);
		if (!Interface) { continue; }

		Interface->ApplyDamaged(GetAttackDamage());
		Interface->ApplyLocalHitPos(Hit.Location);
		Interface->ApplyKnockBack(GetActorLocation());
	}
}

float AAutoAttackHitActor::GetAttackDamage() const
{
	if (!PlayerRuntimeData) { return 1.f; }
	return PlayerRuntimeData->GetFinalAttackPower(AttackPowerScale);
}

