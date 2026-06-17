
#include "FloatingWeaponStandState.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStandState::UFloatingWeaponStandState():
	Phase(0.0f),
	StartPhase(0.0f),
	Frequency(3.0f),
	Amplitude(1.0f)
{
}

void UFloatingWeaponStandState::Initialize()
{
	StartTransformOffset.SetRotation(RelativeRotation.Quaternion());
	
}

void UFloatingWeaponStandState::Start()
{
	TransitionTime = GetStandStateTime();
	Phase = StartPhase;
}

void UFloatingWeaponStandState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	//UE_LOG(LogTemp, Warning, TEXT("StandState"));

	UpdateTransitionTime(DeltaTime);

	RelativeTransform = StartTransformOffset;
	Phase += Frequency * DeltaTime;
	const float ResultOffsetZ = FMath::Sin(Phase) * Amplitude;

	const FVector CurrentLocation = Owner->GetRelativeTransform().GetLocation();
	const FVector ResultLocation = { CurrentLocation.X,
									CurrentLocation.Y,
									CurrentLocation.Z + ResultOffsetZ };
	//UE_LOG(LogTemp, Warning, TEXT("resultOffsetZ %.2f"), resultOffsetZ);

	RelativeTransform.SetLocation(ResultLocation);

	if (IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Transition, EFloatingWeaponState::Attack);
		return;
	}

	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponStandState::GetStartTransformOffset()
{
	FTransform result = StartTransformOffset;
	result.SetRotation(RelativeRotation.Quaternion());
	return result;
}

