
#include "FloatingWeaponAttack.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>


UFloatingWeaponAttack::UFloatingWeaponAttack()
{

}

void UFloatingWeaponAttack::Initialize(AActor* Owner)
{
	UFanAttackBase::Initialize(Owner);

	if (!FloatingWeaponEffect) { return; }
	
	FloatingWeaponEffect->SetOwnerAttack(this);
	FloatingWeaponEffect->SetOwnerActor(Owner);
	FloatingWeaponEffect->Initialize();
	FloatingWeaponEffect->Start(Owner->GetRootComponent());
	
}

void UFloatingWeaponAttack::Update(float DeltaTime, AActor* Player, UEnemyManagerSubsystem* EnemyManager)
{
	UFanAttackBase::Update(DeltaTime, Player, EnemyManager);
	if (FloatingWeaponEffect)
	{
		FloatingWeaponEffect->Update(DeltaTime);
	}
}

bool UFloatingWeaponAttack::UpdateAttack(float DeltaTime)
{
	return UFanAttackBase::UpdateAttack(DeltaTime);
}

bool UFloatingWeaponAttack::IsAttackStateStep()
{
	if (!AutoAttack) { return false; }
	return ElapsedTime < Duration;
}

float UFloatingWeaponAttack::TotalTransitionStateTime()
{
	float resultTime = 0.0f;
	if (!AutoAttack) { return resultTime; }
	const float interval = AutoAttack->GetAutoAttackInterval();
	const float TotalTransitionTimeRatio = 1.0f - StandTimeRatio;
	resultTime = (interval - Duration) * TotalTransitionTimeRatio;
	return resultTime;
}

float UFloatingWeaponAttack::StandStateTime()
{
	float resultTime = 0.0f;
	if (!AutoAttack) { return resultTime; }
	const float interval = AutoAttack->GetAutoAttackInterval();
	resultTime = (interval - Duration) * StandTimeRatio;

	return resultTime;
}
