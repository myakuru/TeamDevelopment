
#include "EffectBase.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UEffectBase::UEffectBase():
	OwnerActor(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	RelativeTransform(FTransform())
{
}

void UEffectBase::Start(USceneComponent* RootComponent)
{
	if (!EffectSystem || EffectComponent) { return; }

	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);

	if (!EffectComponent) { return; }
	EffectComponent->SetRelativeTransform(RelativeTransform);
}

void UEffectBase::DeactivateEffect()
{
	if (!EffectComponent) { return; }
	EffectComponent->Deactivate();
	EffectComponent = nullptr;
}

void UEffectBase::DeactivateImmediateEffect()
{
	if (!EffectComponent) { return; }
	EffectComponent->DeactivateImmediate();
	EffectComponent = nullptr;
}

void UEffectBase::SetAbsolute(
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
