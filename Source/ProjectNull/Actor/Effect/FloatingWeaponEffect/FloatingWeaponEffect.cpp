
#include "FloatingWeaponEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Combat/Attack/FloatingWeaponAttack/FloatingWeaponAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStateBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponAttackState/FloatingWeaponAttackState.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStandState/FloatingWeaponStandState.h>


UFloatingWeaponEffect::UFloatingWeaponEffect():
	OwnerAttack(nullptr),
	OwnerActor(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	RelativeTransform(FTransform()),
	States(TMap<EFloatingWeaponState, TObjectPtr<UFloatingWeaponStateBase>>()),
	CurrentState(nullptr)
{
	
}

void UFloatingWeaponEffect::Initialize()
{
	for (auto& [Type, State] : States)
	{
		if (!State) { continue; }
		State->SetOwner(this);
		State->SetOwnerActor(OwnerActor);
		State->Initialize();
	}
	ChangeState(EFloatingWeaponState::Transition);
}

void UFloatingWeaponEffect::Start(USceneComponent* RootComponent)
{
	if (!CanSpawn()) { return; }

	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);

	//EffectComponent->SetAbsolute(false, false, false);
}

void UFloatingWeaponEffect::Update(float DeltaTime)
{
	if (!OwnerActor || !OwnerAttack || !CurrentState) { return; }

	CurrentState->Update(DeltaTime);

	UpdateTransform();
}

void UFloatingWeaponEffect::ChangeState(EFloatingWeaponState NextState)
{
	if (!States.Contains(NextState) || !States[NextState]) { return; }
	CurrentState = States[NextState];

	if (CurrentState)
	{
		CurrentState->Start();
	}
}

void UFloatingWeaponEffect::ChangeState(EFloatingWeaponState NextState, EFloatingWeaponState TheStateAfterTheNext)
{
	if (!States.Contains(NextState) || !States[NextState]) { return; }
	CurrentState = States[NextState];

	if (CurrentState)
	{
		CurrentState->Start(TheStateAfterTheNext);
	}
}

bool UFloatingWeaponEffect::IsAttackStateStep() const
{
	if (!OwnerAttack) { return false; }
	return OwnerAttack->IsAttackStateStep();
}

FTransform UFloatingWeaponEffect::GetAttackStartTransformOffset()
{
	if (!OwnerActor || !OwnerAttack || !States.Contains(EFloatingWeaponState::Attack)
		|| !States[EFloatingWeaponState::Attack]) { return FTransform(); }
	auto* AttakState = Cast<UFloatingWeaponAttackState>(States[EFloatingWeaponState::Attack]);
	FTransform ResultTransform;
	if (!AttakState) { return ResultTransform; }

	ResultTransform = AttakState->CalcAttackStateTransformOffset(
		OwnerAttack,
		OwnerAttack->GetStartAngle());
	return ResultTransform;
}

FTransform UFloatingWeaponEffect::GetStandStartTransformOffset()
{
	if(!States.Contains(EFloatingWeaponState::Stand)
		|| !States[EFloatingWeaponState::Stand]) { return FTransform(); }
	auto* StandState = Cast<UFloatingWeaponStandState>(States[EFloatingWeaponState::Stand]);
	if (!StandState) { return FTransform(); }
	return StandState->GetStartTransformOffset();
}

void UFloatingWeaponEffect::UpdateTransform()
{
	if (!EffectComponent)	{ return; }
	
	EffectComponent->SetRelativeTransform(RelativeTransform);
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
