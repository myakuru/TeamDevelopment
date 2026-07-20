
#include "FloatingWeaponStateBase.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FloatingWeaponAttack/FloatingWeaponAttack.h>

UFloatingWeaponStateBase::UFloatingWeaponStateBase():
	Owner(nullptr),
	OwnerActor(nullptr),
	RadiusOffset(0.f),
	TransitionTime(0.0f),
	RelativeRotation(FRotator()),
	RelativeTransform(FTransform())
{
}

void UFloatingWeaponStateBase::Update(float DeltaTime)
{
	if (!Owner) { return; }
	Owner->SetRelativeTransform(RelativeTransform);
}

float UFloatingWeaponStateBase::GetTransitionStateTime() const
{
	if (!Owner)			{ return 0.f; }

	auto OwnerAttack = Owner->GetOwnerAttack();
	if (!OwnerAttack)	{ return 0.f; }

	return OwnerAttack->TotalTransitionStateTime() * 0.5f;
}

float UFloatingWeaponStateBase::GetStandStateTime() const
{
	if (!Owner)			{ return 0.f; }

	auto OwnerAttack = Owner->GetOwnerAttack();
	if (!OwnerAttack)	{ return 0.f; }

	return OwnerAttack->StandStateTime();
}

void UFloatingWeaponStateBase::UpdateTransitionTime(float DeltaTime)
{
	if (TransitionTime <= 0.f) { return; }
	TransitionTime -= DeltaTime;
	TransitionTime = std::max(TransitionTime, 0.f);
}
