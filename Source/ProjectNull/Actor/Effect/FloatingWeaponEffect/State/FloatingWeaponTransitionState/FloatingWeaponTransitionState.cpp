

#include "FloatingWeaponTransitionState.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>


UFloatingWeaponTransitionState::UFloatingWeaponTransitionState():
	NextState(EFloatingWeaponState::Attack)
{
}

void UFloatingWeaponTransitionState::Start(EFloatingWeaponState SetNextState)
{
	NextState		= SetNextState;
	TransitionTime	= GetTransitionStateTime();
}

void UFloatingWeaponTransitionState::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }
	UE_LOG(LogTemp, Warning, TEXT("TransitionState"));

	auto* attack = Owner->GetOwnerAttack();

	UpdateTransitionTime(DeltaTime);

	
	if (NextState == EFloatingWeaponState::Attack
		&& attack->IsAttackStateStep())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}

	if(NextState == EFloatingWeaponState::Stand
		&& IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}
