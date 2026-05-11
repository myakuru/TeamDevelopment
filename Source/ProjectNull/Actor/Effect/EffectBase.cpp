
#include "EffectBase.h"


#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UEffectBase::UEffectBase()
{
}

void UEffectBase::Start(USceneComponent* RootComponent)
{
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