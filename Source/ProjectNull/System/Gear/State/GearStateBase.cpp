
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

void UGearStateBase::InterpToStartControlRotation(const FQuat& InCurrentQuaternion, float InLerpAlpha)
{
	if (!Player || !Player->GetController()) { return; }

	CameraRestoreElapsedTime = Owner->GetElapsedTime() - (Owner->GetGearDuration(3) - CameraRestoreDuration);
	float LerpAlpha = CameraRestoreElapsedTime / CameraRestoreDuration;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	const FQuat ResultQuaternion = FQuat::Slerp(InCurrentQuaternion, StartControlRotation.Quaternion(), LerpAlpha);
	Player->GetController()->SetControlRotation(ResultQuaternion.Rotator());

}

void UGearStateBase::InterpToStartTargetArmLength(float InCurrentTargetArmLength, float InLerpAlpha)
{
	if (!Player || !Player->GetSpringArmComponent()) { return; }

	Player->GetSpringArmComponent()->TargetArmLength = FMath::Lerp(InCurrentTargetArmLength, StartTargetArmLength, InLerpAlpha);
}


