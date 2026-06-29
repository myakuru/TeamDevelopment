#include "Laserbeam.h"

#include "Components/BoxComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Effect/EffectBase.h>


ALaserbeam::ALaserbeam()
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
		ECC_GameTraceChannel1,
		ECR_Overlap);

	BoxComp->OnComponentBeginOverlap.AddDynamic(
		this,
		&ALaserbeam::OnLaserBeginOverlap);
}

void ALaserbeam::BeginPlay()
{
	Super::BeginPlay();

	if (NiagaraEffect) { NiagaraEffect->Start(RootComponent); }
}

void ALaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALaserbeam::OnLaserBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OtherActor ||
		OtherActor == this) { return; }

	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	{
		Interface->ApplyDamaged();
		Interface->ApplyKnockBack(GetActorLocation());
	}
}


void ALaserbeam::SetCollisionEnabled(const ECollisionEnabled::Type CollisionType)
{
	if (!BoxComp) { return; }
	BoxComp->SetCollisionEnabled(CollisionType);
}
