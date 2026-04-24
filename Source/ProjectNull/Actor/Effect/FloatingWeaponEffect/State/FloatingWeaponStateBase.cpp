
#include "FloatingWeaponStateBase.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>

UFloatingWeaponStateBase::UFloatingWeaponStateBase():
	TransitionTime(0.0f)
{
}

void UFloatingWeaponStateBase::Update(float DeltaTime)
{
	if (!Owner) { return; }
	Owner->SetLocationOffset(LocationOffset);
}

float UFloatingWeaponStateBase::GetTransitionStateTime() const
{
	if (!Owner || !Owner->GetOwnerAttack()) { return 0.0f; }
	return Owner->GetOwnerAttack()->TransitionStateTime();
}

void UFloatingWeaponStateBase::UpdateTransitionTime(float DeltaTime)
{
	if (TransitionTime <= 0.0f) { return; }
	TransitionTime -= DeltaTime;
	TransitionTime = std::max(TransitionTime, 0.0f);
}
