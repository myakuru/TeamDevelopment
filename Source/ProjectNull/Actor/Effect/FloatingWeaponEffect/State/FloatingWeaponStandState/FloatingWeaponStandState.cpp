

#include "FloatingWeaponStandState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStandState::UFloatingWeaponStandState()
{
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

	Transform = Owner->GetStandStartTransform();

	UFloatingWeaponStateBase::Update(DeltaTime);
}

