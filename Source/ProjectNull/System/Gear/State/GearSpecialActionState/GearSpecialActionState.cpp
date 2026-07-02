
#include "GearSpecialActionState.h"

#include <GameFramework/SpringArmComponent.h>


#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>


UGearSpecialAction::UGearSpecialAction():
	StartControlRotation(FRotator::ZeroRotator),
	RestoreStartControlRotation(FRotator::ZeroRotator),
	StartTargetArmLength(0.f),
	RestoreStartTargetArmLength(0.f),
	CameraRestoreElapsedTime(0.f)
{

}


void UGearSpecialAction::SaveCameraStatus(APlayerBase* Player)
{
	if (!Player)		{ return; }

	const auto SpringArmComp = Player->GetSpringArmComponent();
	if (!SpringArmComp) { return; }

	const auto Controller = Player->GetController();
	if (!Controller)	{ return; }

	StartTargetArmLength = SpringArmComp->TargetArmLength;
	StartControlRotation = Controller->GetControlRotation();
}

void UGearSpecialAction::UpdateCameraRestoreInterpolation(
	APlayerBase* Player,
	UGearBase* Owner,
	int32 LevelIndex,
	float CameraRestoreDuration,
	float DeltaTime)
{
	if (!Owner) { return; }

	// カメラ復帰経過時間更新
	CameraRestoreElapsedTime = Owner->GetElapsedTime() - (Owner->GetGearDuration(LevelIndex) - CameraRestoreDuration);

	if (CameraRestoreElapsedTime < 0.0f) { return; }

	// 補間値
	float LerpAlpha = CameraRestoreElapsedTime / CameraRestoreDuration;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	// 回転とプレイヤーとカメラ距離を補間する
	InterpToStartControlRotation(Player,RestoreStartControlRotation.Quaternion(), LerpAlpha);
	InterpToStartTargetArmLength(Player,RestoreStartTargetArmLength, LerpAlpha);
}

void UGearSpecialAction::InterpToStartControlRotation(
	APlayerBase* Player,
	const FQuat& InCurrentQuaternion,
	float InLerpAlpha)
{
	if (!Player)		{ return; }

	const auto Controller = Player->GetController();
	if (!Controller)	{ return; }

	// 球面補間を行い、カメラ回転を更新する
	const FQuat ResultQuaternion = FQuat::Slerp(
		InCurrentQuaternion,
		StartControlRotation.Quaternion(),
		InLerpAlpha);

	Controller->SetControlRotation(ResultQuaternion.Rotator());
}

void UGearSpecialAction::InterpToStartTargetArmLength(
	APlayerBase* Player,
	float InCurrentTargetArmLength,
	float InLerpAlpha)
{
	if (!Player)		{ return; }
	
	const auto SpringArmComp = Player->GetSpringArmComponent();
	if (!SpringArmComp) { return; }

	// 線形補間を行い、カメラ距離更新する
	SpringArmComp->TargetArmLength = FMath::Lerp(
		InCurrentTargetArmLength,
		StartTargetArmLength,
		InLerpAlpha);
}
