
#include "Laserbeam.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Actor/Effect/EffectBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

ALaserbeam::ALaserbeam():
	BoxComp(nullptr),
	NiagaraEffectArray(TArray<TObjectPtr<UEffectBase>>()),
	PlayerRuntimeData(nullptr),
	AttackPowerScale(1.f)
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	if (!BoxComp) { return; }

	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetGenerateOverlapEvents(true);
	BoxComp->SetCollisionResponseToChannel(
		ECC_GameTraceChannel1,
		ECR_Overlap);

	BoxComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&ALaserbeam::OnLaserbeamBeginOverlap);
}

void ALaserbeam::BeginPlay()
{
	Super::BeginPlay();
	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }
	PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
}

void ALaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// DrawDebugBox(GetWorld(),
	// 	BoxComp->GetComponentLocation(),
	// 			BoxComp->GetScaledBoxExtent(),
	// 				GetActorQuat(),
	// 				FColor::Green);
}

void ALaserbeam::OnLaserbeamBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor ||
		OtherActor == this) {
		return;
	}

	if (!PlayerRuntimeData) { return;}
	
	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	{
		Interface->ApplyDamaged(PlayerRuntimeData->GetFinalAttackPower(AttackPowerScale));
		Interface->ApplyKnockBack(GetActorLocation());
	}
}


void ALaserbeam::SetLaserEnabled(bool bEnabled)
{
	if (!BoxComp) { return; }

	const ECollisionEnabled::Type CollisionType =
		bEnabled
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision;

	BoxComp->SetCollisionEnabled(CollisionType);

	for (auto& Effect : NiagaraEffectArray)
	{
		if (bEnabled)
		{
			Effect->Start(RootComponent);
		}
		else
		{
			Effect->DeactivateEffect();
		}
	}
}