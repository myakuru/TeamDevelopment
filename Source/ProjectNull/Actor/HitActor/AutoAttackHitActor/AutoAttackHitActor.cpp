
#include "AutoAttackHitActor.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

AAutoAttackHitActor::AAutoAttackHitActor()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	if (!BoxComp) { return; }
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly);

	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionResponseToChannel(
		ECC_Enemy,
		ECR_Overlap);

	BoxComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&AAutoAttackHitActor::OnAutoAttackBeginOverlap);
}

void AAutoAttackHitActor::BeginPlay()
{
	Super::BeginPlay();

	// ゲームインスタンス取得
	GameInstance = Cast<USuperGameInstance>(GetWorld()->GetGameInstance());

	if (!BoxComp) { return; }
	PreviousLocation = BoxComp->GetComponentLocation();
}

void AAutoAttackHitActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!BoxComp) { return; }

	PerformHitSweep();

	PreviousLocation = BoxComp->GetComponentLocation();

	HitActors.Empty();
}

void AAutoAttackHitActor::SetHitEnabled(bool bEnabled)
{
	const ECollisionEnabled::Type CollisionType =
		bEnabled
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision;

	if (!BoxComp) { return; }
	BoxComp->SetCollisionEnabled(CollisionType);
}

void AAutoAttackHitActor::OnAutoAttackBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//// キャラクターインターフェースを実装しているか
	//if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	//{
	//	Interface->ApplyDamaged();
	//	Interface->ApplyKnockBack(GetActorLocation());
	//}
}

void AAutoAttackHitActor::PerformHitSweep()
{
	if (!BoxComp) { return; }

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
		if (HitActors.Contains(HitActor))
		{
			continue;
		}

		HitActors.Add(HitActor);

		auto* Interface = Cast<ICharacterInterface>(HitActor);
		if (!Interface) { continue; }

		Interface->ApplyDamaged(SetAttackDamage());
		Interface->ApplyKnockBack(GetActorLocation(),100.f);
		UE_LOG(LogTemp, Display, TEXT("当たった"));
	}
}

float AAutoAttackHitActor::SetAttackDamage()
{
	if (GameInstance)
	{
		PlayerRuntimeData = GameInstance->GetPlayerRuntimeData();

		if (PlayerRuntimeData)
		{
			float AttackDamage = PlayerRuntimeData->GetPlayerAttackDamage();

			return AttackDamage;
		}
	}

	return 1.0f;
}

