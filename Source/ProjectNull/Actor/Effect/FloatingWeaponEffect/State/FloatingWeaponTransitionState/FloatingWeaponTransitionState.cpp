

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

	UpdateTransitionTime(DeltaTime);
	
	switch (NextState)
	{
	case EFloatingWeaponState::Stand:	UpdateStandTransition(OwnerActor, DeltaTime);	break;
	case EFloatingWeaponState::Attack:	UpdateAttakTransition(OwnerActor,DeltaTime);	break;
	case EFloatingWeaponState::Transition: break;
	case EFloatingWeaponState::Count: break;
	default: break;
	}

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}

void UFloatingWeaponTransitionState::UpdateAttakTransition(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	const FTransform currentTransform = Owner->GetTransform();
	const FTransform targetTransform = Owner->GetAttackStartTransform(OwnerActor);
	const FVector currentLocation = currentTransform.GetLocation();
	const FVector targetLocation = targetTransform.GetLocation();
	const float lerpValue = TransitionTime / GetTransitionStateTime();
	const FVector resultLocation = FMath::Lerp(currentLocation,targetLocation,lerpValue);
	//Transform = FMath::Lerp(currentTransform, targetTransform, lerpValue);
	Transform.SetLocation(resultLocation);
	if (Owner->GetOwnerAttack()->IsAttackStateStep())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}
}

void UFloatingWeaponTransitionState::UpdateStandTransition(AActor* OwnerActor, float DeltaTime)
{

	const FTransform currentTransform = Owner->GetTransform();
	const FTransform targetTransform = Owner->GetStandStartTransform(OwnerActor);
	const FVector currentLocation = currentTransform.GetLocation();
	const FVector targetLocation = targetTransform.GetLocation();
	const float lerpValue = TransitionTime / GetTransitionStateTime();
	const FVector resultLocation = FMath::Lerp(currentLocation, targetLocation, lerpValue);
	//Transform = FMath::Lerp(currentTransform, targetTransform, lerpValue);
	Transform.SetLocation(resultLocation);

	if (IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}
}
