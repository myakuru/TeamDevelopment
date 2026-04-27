
#include "FloatingWeaponEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStateBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponAttackState/FloatingWeaponAttackState.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStandState/FloatingWeaponStandState.h>


UFloatingWeaponEffect::UFloatingWeaponEffect():
	OwnerAttack(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	Transform(FTransform()),
	RadiusOffset(200.0f),
	Rotation(FRotator())
{
	
}

void UFloatingWeaponEffect::Initialize()
{
	for (auto& [type, state] : States)
	{
		if (!state) { continue; }
		state->SetOwner(this);
		state->SetOwnerActor(OwnerActor);
	}
	ChangeState(EFloatingWeaponState::Transition);
}

void UFloatingWeaponEffect::Start(USceneComponent* RootComponent)
{
	if (!CanSpawn()) { return; }

	// ï¿½Gï¿½tï¿½Fï¿½Nï¿½gï¿½ÌÄï¿½ï¿½Jï¿½n
	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
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
	auto* attakState = Cast<UFloatingWeaponAttackState>(States[EFloatingWeaponState::Attack]);
	FTransform resultTransform;
	if (!attakState) { return resultTransform; }

	resultTransform = attakState->CalcAttackStateTransformOffset(OwnerAttack, OwnerAttack->StartAngle).Transform;
	return resultTransform;
}

FTransform UFloatingWeaponEffect::GetStandStartTransformOffset()
{
	if(!States.Contains(EFloatingWeaponState::Stand)
		|| !States[EFloatingWeaponState::Stand]) { return FTransform(); }
	const auto* standState = Cast<UFloatingWeaponStandState>(States[EFloatingWeaponState::Stand]);
	if (!standState) { return FTransform(); }
	return standState->GetStartTransformOffset();
}

void UFloatingWeaponEffect::UpdateTransform()
{
	CalcTransformOffset();

	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::CalcTransformOffset()
{
	if (!OwnerActor) { return; }

	// ƒvƒŒƒCƒ„[‚ÌÀ•W
	const FVector playerLocation	= OwnerActor->GetActorLocation();
	// ƒvƒŒƒCƒ„[‚Ì‰ñ“]
	FRotator playerRotation			= OwnerActor->GetActorRotation();

	// ƒ[ƒ‹ƒhƒIƒtƒZƒbƒgÀ•W
	const FVector worldOffsetLocation	= playerRotation.RotateVector(LocationOffset);
	const FVector resultLocation		= playerLocation + worldOffsetLocation;

	// ƒƒ‚:À•W‚Æ“¯‚¶‚æ‚¤‚É‰ñ“]ƒIƒtƒZƒbƒgŒvŽZŒãƒvƒŒƒCƒ„[‚Ì‰ñ“]‚ðl—¶
	Rotation.Yaw = playerRotation.Yaw + RotatorYawOffset;

	Transform.SetRotation(Rotation.Quaternion());
	Transform.SetLocation(resultLocation);	
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
