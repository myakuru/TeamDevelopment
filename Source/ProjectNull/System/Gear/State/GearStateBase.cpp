
#include "GearStateBase.h"

#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>

UGearStateBase::UGearStateBase():
	Player(nullptr),
	GearComponent(nullptr),
	Owner(nullptr),
	StartControlRotation(FRotator::ZeroRotator),
	StartTargetArmLength(0.f),
	CameraRestoreDuration(0.f)
{
	
}

void UGearStateBase::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	Player			= InPlayer;
	GearComponent	= InGearComponent;
	Owner			= InOwner;
}

void UGearStateBase::SaveCameraStatus(APlayerBase* InPlayer)
{
	if (!InPlayer || !InPlayer->GetSpringArmComponent() || !InPlayer->GetController()) { return; }
	
	StartTargetArmLength = InPlayer->GetSpringArmComponent()->TargetArmLength;
	StartControlRotation = InPlayer->GetController()->GetControlRotation();
}

void UGearStateBase::UpdateCameraRestoreInterpolation(float DeltaTime)
{
	if (!Owner) { return; }

	CameraRestoreElapsedTime = Owner->GetElapsedTime() - (Owner->GetGearDuration(3) - CameraRestoreDuration);

	if (CameraRestoreElapsedTime < 0.0f) { return; }


	float LerpAlpha = CameraRestoreElapsedTime / CameraRestoreDuration;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	InterpToStartControlRotation(RestoreStartControlRotation.Quaternion(), LerpAlpha);
	InterpToStartTargetArmLength(RestoreStartTargetArmLength, LerpAlpha);
}

void UGearStateBase::InterpToStartControlRotation(
	const FQuat& InCurrentQuaternion,
	float InLerpAlpha)
{
	if (!Player || !Player->GetController()) { return; }


	const FQuat ResultQuaternion = FQuat::Slerp(
		InCurrentQuaternion,
		StartControlRotation.Quaternion(),
		InLerpAlpha);

	Player->GetController()->SetControlRotation(ResultQuaternion.Rotator());

}

void UGearStateBase::InterpToStartTargetArmLength(
	float InCurrentTargetArmLength,
	float InLerpAlpha)
{
	if (!Player || !Player->GetSpringArmComponent()) { return; }

	Player->GetSpringArmComponent()->TargetArmLength = FMath::Lerp(InCurrentTargetArmLength, StartTargetArmLength, InLerpAlpha);
}


