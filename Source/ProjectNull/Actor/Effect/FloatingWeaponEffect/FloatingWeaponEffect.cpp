
#include "FloatingWeaponEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStateBase.h>


UFloatingWeaponEffect::UFloatingWeaponEffect():
	OwnerAttack(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	Transform(FTransform()),
	RadiusOffset(200.0f),
	RotatorOffset(FRotator())
{
	
}

void UFloatingWeaponEffect::Initialize()
{
	for (auto& [type, state] : States)
	{
		if (!state) { continue; }
		state->SetOnwer(this);
	}
	ChangeState(EFloatingWeaponState::Stand);
}

void UFloatingWeaponEffect::Start(USceneComponent* RootComponent)
{
	if (!CanSpawn()) { return; }

	//　エフェクトの再生開始
	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void UFloatingWeaponEffect::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !OwnerAttack || !CurrentState) { return; }

	CurrentState->Update(OwnerActor,DeltaTime);

	UpdateTransform();
}

void UFloatingWeaponEffect::ChangeState(EFloatingWeaponState State)
{
	if (!States.Contains(State) || !States[State]) { return; }
	CurrentState = States[State];
}

void UFloatingWeaponEffect::UpdateTransform()
{
	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
