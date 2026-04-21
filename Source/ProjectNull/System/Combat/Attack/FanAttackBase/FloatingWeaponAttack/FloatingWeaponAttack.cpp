
#include "FloatingWeaponAttack.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>


UFloatingWeaponAttack::UFloatingWeaponAttack()
{

}

void UFloatingWeaponAttack::Initialize(AActor* Owner)
{
	UFanAttackBase::Initialize(Owner);

	if (FloatingWeaponEffect)
	{
		FloatingWeaponEffect->SetOwnerAttack(this);
		FloatingWeaponEffect->Initialize();
		FloatingWeaponEffect->Start(Owner->GetRootComponent());
	}

}

void UFloatingWeaponAttack::Update(float DeltaTime, AActor* Player, UEnemyManagerSubsystem* EnemyManager)
{
	UFanAttackBase::Update(DeltaTime, Player, EnemyManager);
	if (FloatingWeaponEffect)
	{
		FloatingWeaponEffect->Update(OwnerActor, DeltaTime);
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

float UFloatingWeaponAttack::TransitionStateTime()
{
	float resultTime = 0.0f;
	if (!AutoAttack) { return resultTime; }
	const float interval = AutoAttack->GetAutoAttackInterval();
	resultTime = (interval - Duration) / 3.0f;

	return resultTime;
}
