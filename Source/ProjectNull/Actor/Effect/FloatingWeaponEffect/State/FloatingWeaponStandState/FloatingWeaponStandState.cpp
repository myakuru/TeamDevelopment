
#include "FloatingWeaponStandState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStandState::UFloatingWeaponStandState():
	Phase(0.0f),
	Frequency(0.0f),
	Amplitude(0.0f)
{
}

void UFloatingWeaponStandState::Initialize()
{
	StartTransformOffset.SetRotation(RelativeRotation.Quaternion());

}

void UFloatingWeaponStandState::Start()
{
	TransitionTime = GetTransitionStateTime();
}

void UFloatingWeaponStandState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("StandState"));

	UpdateTransitionTime(DeltaTime);

	Phase += Frequency * DeltaTime;
	const float resultOffsetZ = FMath::Sin(Phase) * Amplitude;

	const FVector currentLocation = Owner->GetRelativeTransform().GetLocation();
	const FVector resultLocation = { currentLocation.X,
									currentLocation.Y,
									currentLocation.Z + resultOffsetZ };
	//UE_LOG(LogTemp, Warning, TEXT("resultOffsetZ %.2f"), resultOffsetZ);

	RelativeTransform.SetLocation(resultLocation);

	if (IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Transition, EFloatingWeaponState::Attack);
		return;
	}
	//RelativeTransform.SetRotation(RelativeRotation.Quaternion());
	//RelativeTransform = StartTransformOffset;

	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponStandState::GetStartTransformOffset()
{
	FTransform result = StartTransformOffset;
	result.SetRotation(RelativeRotation.Quaternion());
	return result;
}

