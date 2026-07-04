
#include "NiagaraEffectBase.h"


#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"


ANiagaraEffectBase::ANiagaraEffectBase():
	OwnerActor(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	RelativeTransform(FTransform())
{
	PrimaryActorTick.bCanEverTick = true;

}

void ANiagaraEffectBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ANiagaraEffectBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ANiagaraEffectBase::Start(USceneComponent* InRootComponent)
{
	if (!EffectSystem) { return; }

	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		InRootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);

	if (!EffectComponent) { return; }
	EffectComponent->SetRelativeTransform(RelativeTransform);
}

void ANiagaraEffectBase::DeactivateEffect()
{
	if (!EffectComponent) { return; }
	EffectComponent->Deactivate();
	EffectComponent = nullptr;
}

void ANiagaraEffectBase::SetAbsolute(
	bool bNewAbsoluteLocation,
	bool bNewAbsoluteRotation,
	bool bNewAbsoluteScale)
{
	if (!EffectComponent) { return; }
	EffectComponent->SetAbsolute(
		bNewAbsoluteLocation,
		bNewAbsoluteRotation,
		bNewAbsoluteScale);
}

