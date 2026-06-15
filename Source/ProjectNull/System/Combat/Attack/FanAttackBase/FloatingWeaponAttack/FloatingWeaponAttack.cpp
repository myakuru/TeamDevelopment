
#include "FloatingWeaponAttack.h"

#include <ProjectNull/Actor/Effect/SlashEffect/SlashEffectBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>


UFloatingWeaponAttack::UFloatingWeaponAttack()
{

}

void UFloatingWeaponAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	UFanAttackBase::Initialize(Owner);

	if (!FloatingWeaponEffect) { return; }
	
	FloatingWeaponEffect->SetOwnerAttack(this);
	FloatingWeaponEffect->SetOwnerActor(Owner);
	FloatingWeaponEffect->Initialize();
	FloatingWeaponEffect->Start(Owner->GetRootComponent());
	
}

void UFloatingWeaponAttack::Start()
{
	auto Owner = GetOwnerActor();
	if (!Owner) { return; }

	UFanAttackBase::Start();

	for (TObjectPtr<UEffectBase> SlashEffect : SlashEffectArray) 
	{
		if (!SlashEffect) { continue; }
		SlashEffect->Start(Owner->GetRootComponent());
	}
}

void UFloatingWeaponAttack::Update(float DeltaTime)
{
	UFanAttackBase::Update(DeltaTime);

	if (FloatingWeaponEffect)
	{
		FloatingWeaponEffect->Update(DeltaTime);
	}
}

bool UFloatingWeaponAttack::IsAttackStateStep()
{
	if (!AutoAttack) { return false; }
	return ElapsedTime < Duration;
}

float UFloatingWeaponAttack::TotalTransitionStateTime()
{
	float ResultTime = 0.0f;
	if (!AutoAttack) { return ResultTime; }
	const float Interval = AutoAttack->GetAutoAttackInterval();
	const float TotalTransitionTimeRatio = 1.0f - StandTimeRatio;
	ResultTime = (Interval - Duration) * TotalTransitionTimeRatio;
	return ResultTime;
}

float UFloatingWeaponAttack::StandStateTime()
{
	float ResultTime = 0.0f;
	if (!AutoAttack) { return ResultTime; }
	const float Interval = AutoAttack->GetAutoAttackInterval();
	ResultTime = (Interval - Duration) * StandTimeRatio;

	return ResultTime;
}
