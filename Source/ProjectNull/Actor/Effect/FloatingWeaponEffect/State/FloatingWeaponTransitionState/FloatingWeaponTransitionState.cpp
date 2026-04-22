

#include "FloatingWeaponTransitionState.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>


UFloatingWeaponTransitionState::UFloatingWeaponTransitionState()
{
}

void UFloatingWeaponTransitionState::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	auto* attack = Owner->GetOwnerAttack();

	if (attack->IsActiveFirstFrame())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}

	if (attack->CanDeactivate())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}
}
