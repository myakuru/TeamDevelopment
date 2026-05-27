
#include "GearStateBase.h"

#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>

UGearStateBase::UGearStateBase():
	Player(nullptr),
	GearComponent(nullptr),
	Owner(nullptr),
	StartControlRotation(FRotator::ZeroRotator),
	RestoreStartControlRotation(FRotator::ZeroRotator),
	StartTargetArmLength(0.f),
	RestoreStartTargetArmLength(0.f),
	CameraRestoreDuration(0.f),
	CameraRestoreElapsedTime(0.f)
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

void UGearStateBase::SaveCameraStatus()
{
	if (!Player || !Player->GetSpringArmComponent() || !Player->GetController()) { return; }

	StartTargetArmLength = Player->GetSpringArmComponent()->TargetArmLength;
	StartControlRotation = Player->GetController()->GetControlRotation();
}

void UGearStateBase::UpdateCameraRestoreInterpolation(float DeltaTime)
{
	if (!Owner) { return; }

	// ギアレベルのインデックス
	const int32 LevelIndex = GetGearLevelIndex();

	// カメラ復帰経過時間更新
	CameraRestoreElapsedTime = Owner->GetElapsedTime() - (Owner->GetGearDuration(LevelIndex) - CameraRestoreDuration);

	if (CameraRestoreElapsedTime < 0.0f) { return; }

	// 補間値
	float LerpAlpha = CameraRestoreElapsedTime / CameraRestoreDuration;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	// 回転とプレイヤーとカメラ距離を補間する
	InterpToStartControlRotation(RestoreStartControlRotation.Quaternion(), LerpAlpha);
	InterpToStartTargetArmLength(RestoreStartTargetArmLength, LerpAlpha);
}

void UGearStateBase::InterpToStartControlRotation(
	const FQuat& InCurrentQuaternion,
	float InLerpAlpha)
{
	if (!Player || !Player->GetController()) { return; }

	// 球面補間を行い、カメラ回転を更新する
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

	// 線形補間を行い、カメラ距離更新する
	Player->GetSpringArmComponent()->TargetArmLength = FMath::Lerp(
		InCurrentTargetArmLength,
		StartTargetArmLength,
		InLerpAlpha);
}


