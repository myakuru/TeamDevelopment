
#include "FloatingWeaponStandState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStandState::UFloatingWeaponStandState()
{
}

void UFloatingWeaponStandState::Initialize()
{
	StartTransformOffset.SetRotation(RelativeRotation.Quaternion());

}

void UFloatingWeaponStandState::Start()
{
	TransitionTime = GetTransitionStateTime();
}

void UFloatingWeaponStandState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("StandState"));

	UpdateTransitionTime(DeltaTime);

	if (IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Transition, EFloatingWeaponState::Attack);
		return;
	}
	StartTransformOffset.SetRotation(RelativeRotation.Quaternion());
	RelativeTransform = StartTransformOffset;

	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponStandState::GetStartTransformOffset()
{
	FTransform result = StartTransformOffset;
	result.SetRotation(RelativeRotation.Quaternion());
	return result;
}

