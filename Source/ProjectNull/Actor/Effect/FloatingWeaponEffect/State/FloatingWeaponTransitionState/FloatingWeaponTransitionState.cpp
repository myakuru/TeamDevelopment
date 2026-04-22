

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

	if (!Owner) { return; }
	//オフセットがほしい
	StartTransformOffset = Owner->GetTransform();
}

void UFloatingWeaponTransitionState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }
	UE_LOG(LogTemp, Warning, TEXT("TransitionState"));

	UpdateTransitionTime(DeltaTime);
	
	switch (NextState)
	{
	case EFloatingWeaponState::Stand:
		UpdateStandTransition(DeltaTime);

		if (IsFinishedTransitionState())
		{
			Owner->ChangeState(EFloatingWeaponState::Stand);
			return;
		}
		
		break;
	case EFloatingWeaponState::Attack:
		UpdateAttakTransition(DeltaTime);
		if (Owner->GetOwnerAttack()->IsAttackStateStep())
		{
			Owner->ChangeState(EFloatingWeaponState::Attack);
			return;
		}

		break;
	case EFloatingWeaponState::Transition: break;
	case EFloatingWeaponState::Count: break;
	default: break;
	}

	UFloatingWeaponStateBase::Update(DeltaTime);
}

void UFloatingWeaponTransitionState::UpdateAttakTransition(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	/*const float lerpValue = TransitionTime / GetTransitionStateTime();
	const FTransform currentTransform = StartTransformOffset;
	const FTransform targetTransform = Owner->GetAttackStartTransform();

	const FVector currentLocation = currentTransform.GetLocation();
	const FVector targetLocation = targetTransform.GetLocation();
	const FVector resultLocation = FMath::Lerp(currentLocation,targetLocation,lerpValue);

	const FQuat4d currentQuaternion = currentTransform.GetRotation().Rotator().Quaternion();
	const FQuat4d targetQuaternion = targetTransform.GetRotation().Rotator().Quaternion();
	const FQuat4d resultQuaternion = FQuat4d::Slerp(currentQuaternion, targetQuaternion, lerpValue);


	Transform.SetLocation(resultLocation);
	Transform.SetRotation(resultQuaternion);*/

}

void UFloatingWeaponTransitionState::UpdateStandTransition(float DeltaTime)
{
	// オフセットの補間→最終的にプレイヤー
	const float lerpValue = TransitionTime / GetTransitionStateTime();

	const FTransform currentTransform = StartTransformOffset;
	const FTransform targetTransform = Owner->GetStandStartTransform();
	
	const FVector currentLocation = currentTransform.GetLocation();
	const FVector targetLocation = targetTransform.GetLocation();
	const FVector resultLocation = FMath::Lerp(currentLocation, targetLocation, lerpValue);

	const FQuat4d currentQuaternion = currentTransform.GetRotation().Rotator().Quaternion();
	const FQuat4d targetQuaternion = targetTransform.GetRotation().Rotator().Quaternion();
	const FQuat4d resultQuaternion = FQuat4d::Slerp(currentQuaternion, targetQuaternion, lerpValue);

	/*Transform.SetLocation(resultLocation);
	Transform.SetRotation(resultQuaternion);*/

}