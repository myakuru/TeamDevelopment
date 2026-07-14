
#include "CrossLaserbeam.h"


#include "Components/BoxComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Effect/EffectBase.h>

#include "ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h"
#include "ProjectNull/GameInstance/SuperGameInstance.h"

ACrossLaserbeam::ACrossLaserbeam():
	AttackPowerScale(1.f)
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	BoxCompArray.SetNum(LaserbeamNum);

	for (int32 Index = 0; Index < LaserbeamNum; ++Index)
	{
		auto& BoxComp = BoxCompArray[Index];

		FString Name = FString::Printf(
			TEXT("LaserBox_%d"),
			Index);

		BoxComp = CreateDefaultSubobject<UBoxComponent>(*Name);
		if (!BoxComp) { continue; }
		BoxComp->SetupAttachment(RootComponent);
		BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		BoxComp->SetGenerateOverlapEvents(true);
		BoxComp->SetCollisionResponseToChannel(
			ECC_GameTraceChannel1,
			ECR_Overlap);

		BoxComp->OnComponentBeginOverlap.AddDynamic(
			this,
			&ACrossLaserbeam::OnLaserBeginOverlap);
	}

}

void ACrossLaserbeam::BeginPlay()
{
	Super::BeginPlay();

	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }
	PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
}

void ACrossLaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACrossLaserbeam::SetLaserEnabled(bool bEnabled)
{
	const ECollisionEnabled::Type CollisionType =
		bEnabled
		? ECollisionEnabled::QueryOnly
		: ECollisionEnabled::NoCollision;

	for (auto& BoxComp : BoxCompArray)
	{
		if (!BoxComp) { continue; }

		BoxComp->SetCollisionEnabled(CollisionType);
	}

	

	for (auto& Effect : NiagaraEffectArray)
	{
		if (bEnabled)
		{
			Effect->Start(RootComponent);
		}
		else 
		{
			Effect->DeactivateImmediateEffect();
		}
	}
}


void ACrossLaserbeam::OnLaserBeginOverlap(
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

	if (!PlayerRuntimeData) { return; }
	
	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OtherActor))
	{
		Interface->ApplyDamaged(PlayerRuntimeData->GetFinalAttackPower(AttackPowerScale));
		Interface->ApplyKnockBack(GetActorLocation());
	}
}

