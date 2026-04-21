

#include "FloatingWeaponTransitionState.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>


UFloatingWeaponTransitionState::UFloatingWeaponTransitionState():
	NextState(EFloatingWeaponState::None),
	TransitionTime(0.0f)
{
}

void UFloatingWeaponTransitionState::Start(EFloatingWeaponState SetNextState)
{
	NextState = SetNextState;
	if(Owner && Owner->GetOwnerAttack())
	{
		TransitionTime = Owner->GetOwnerAttack()->TransitionStateTime();
	}
}

void UFloatingWeaponTransitionState::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	auto* attack = Owner->GetOwnerAttack();

	UpdateTransitionTime(DeltaTime);

	
	if (attack->IsAttackStateStep())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}

	if(IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}
}

void UFloatingWeaponTransitionState::UpdateTransitionTime(float DeltaTime)
{
	if (TransitionTime <= 0.0f) { return; }
	TransitionTime -= DeltaTime;
	TransitionTime = std::max(TransitionTime,0.0f);
}
