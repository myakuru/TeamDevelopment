
#include "GearSpecialActionState.h"

#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/System/Controller/RobotController/RobotController.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Gear/State/GearStateBase.h>


UGearSpecialAction::UGearSpecialAction():
	StartControlRotation(FRotator::ZeroRotator),
	RestoreStartControlRotation(FRotator::ZeroRotator),
	StartTargetArmLength(0.f),
	RestoreStartTargetArmLength(0.f),
	CameraRestoreElapsedTime(0.f),
	CameraRestoreDuration(0.f)
{

}

void UGearSpecialAction::Initialize(UGearStateBase* InOwner)
{
	Owner = InOwner;

	if (!Owner) { return; }

	Player = Owner->GetPlayer();
	Gear = Owner->GetGear();

	if (!Player) { return; }
	// ロボットコントローラーの取得
	RobotController = Cast<ARobotController>(Player->GetController());

}

void UGearSpecialAction::Execute(const FTransform& StartTransform)
{
	StartPlayerTransform = StartTransform;
	SaveCameraStatus();
	InitializeRestoreStartData();
}

void UGearSpecialAction::Update(
	float DeltaTime,
	float InElapsedTime)
{
	// カメラ復帰処理更新
	UpdateCameraRestoreInterpolation(DeltaTime);

	// 復元補間中は処理を行わない
	if (GetCameraRestoreElapsedTime() >= 0.0f) { return; }

	// 現在の区間インデックス取得
	const int32 CurrentIndex = GetCurrentSectionIndex(InElapsedTime);

	if (!CameraSequenceData.IsValidIndex(CurrentIndex)) { return; }

	// 現在のカメラデータ取得
	const FCameraSequenceData& CurrentData = CameraSequenceData[CurrentIndex];

	// 区間内での開始時間
	const float SectionStartTime = GetElapsedTimeToIndex(CurrentIndex);

	// 補間値を求める
	const float LerpAlpha = FMath::Clamp(
		(InElapsedTime - SectionStartTime) / CurrentData.Time,
		0.f,
		1.f);

	// カメラの回転更新処理
	UpdateCameraRotation(CurrentIndex, LerpAlpha);

	// プレイヤーとカメラとの距離更新
	UpdateTargetArmLength(CurrentIndex, LerpAlpha);
}

float UGearSpecialAction::GetTotalDuration() const
{
	float TotalDuration = 0.f;

	// 区間時間の合計時間をギア発動時間とする
	for (auto& Data : CameraSequenceData) 
	{
		TotalDuration += Data.Time;
	}

	TotalDuration += CameraRestoreDuration;

	return TotalDuration;
}

void UGearSpecialAction::SaveCameraStatus()
{
	if (!Player)		{ return; }

	const auto SpringArmComp = Player->GetSpringArmComponent();
	if (!SpringArmComp) { return; }

	const auto Controller = Player->GetController();
	if (!Controller)	{ return; }

	StartTargetArmLength = SpringArmComp->TargetArmLength;
	StartControlRotation = Controller->GetControlRotation();
}


void UGearSpecialAction::InitializeRestoreStartData()
{
	// 最後の要素番号を取り出す
	const int32 MaxIndex = CameraSequenceData.Num() - 1;

	if (!CameraSequenceData.IsValidIndex(MaxIndex)) { return; }

	const auto& Data = CameraSequenceData[MaxIndex];

	// ギアスキル発動開始時のプレイヤーYaw回転
	const float BaseYaw = StartPlayerTransform.GetRotation().Rotator().Yaw;

	// 最後の要素データを取得
	RestoreStartTargetArmLength = Data.TargetArmLength;
	RestoreStartControlRotation = Data.TargetRotator;

	// オフセット考慮して計算
	RestoreStartControlRotation.Yaw += BaseYaw;
}


void UGearSpecialAction::UpdateCameraRestoreInterpolation(
	float DeltaTime)
{
	if (!Player ||
		!Gear	||
		!Owner) { return; }

	const int32 LevelIndex = Owner->GetGearLevelIndex();
	
	// カメラ復帰経過時間更新
	CameraRestoreElapsedTime = Gear->GetElapsedTime() - (Gear->GetGearDuration(LevelIndex) - CameraRestoreDuration);

	if (CameraRestoreElapsedTime < 0.0f) { return; }

	// 補間値
	float LerpAlpha = CameraRestoreElapsedTime / CameraRestoreDuration;
	LerpAlpha = FMath::Clamp(LerpAlpha, 0.f, 1.f);

	// 回転とプレイヤーとカメラ距離を補間する
	InterpToStartControlRotation(RestoreStartControlRotation.Quaternion(), LerpAlpha);
	InterpToStartTargetArmLength(RestoreStartTargetArmLength, LerpAlpha);
}

void UGearSpecialAction::UpdateCameraRotation(
	int32 DataIndex,
	float InLerpAlpha)
{
	if (!RobotController) {
		return;
	}

	if (!CameraSequenceData.IsValidIndex(DataIndex)) { return; }

	const FCameraSequenceData& Data = CameraSequenceData[DataIndex];

	if (Data.bPause) { return; }

	// ギアスキル発動開始時のプレイヤーYaw回転
	const float BaseYaw = StartPlayerTransform.GetRotation().Rotator().Yaw;

	// 区間開始の回転
	FRotator StartRotator = StartControlRotation;

	// 前区間のカメラデータ取得
	if (const FCameraSequenceData* PrevData = GetPreviousValidCameraData(DataIndex))
	{
		StartRotator = PrevData->TargetRotator;
	}

	// 開始時のプレイヤーYaw回転を考慮して計算
	StartRotator.Yaw += BaseYaw;

	// 目指すべき回転
	FRotator TargetRotator = Data.TargetRotator;

	// 開始時のプレイヤーYaw回転を考慮して計算
	TargetRotator.Yaw += BaseYaw;

	// 補間処理
	const FQuat ResultQuat = FQuat::Slerp(
		StartRotator.Quaternion(),
		TargetRotator.Quaternion(),
		InLerpAlpha);

	// カメラの回転更新
	RobotController->SetControlRotation(ResultQuat.Rotator());
}

void UGearSpecialAction::UpdateTargetArmLength(
	int32 DataIndex,
	float InLerpAlpha)
{
	if (!Player) {
		return;
	}

	auto SpringArm = Player->GetSpringArmComponent();
	if (!SpringArm) { return; }

	if (!CameraSequenceData.IsValidIndex(DataIndex)) { return; }

	const FCameraSequenceData& Data = CameraSequenceData[DataIndex];

	if (Data.bPause) { return; }

	// 区間開始のカメラとプレイヤーの距離
	float StartArmLength = StartTargetArmLength;

	// 前区間のカメラデータ取得
	if (const FCameraSequenceData* PrevData = GetPreviousValidCameraData(DataIndex))
	{
		StartArmLength = PrevData->TargetArmLength;
	}

	// 補間処理
	SpringArm->TargetArmLength = FMath::Lerp(
		StartArmLength,
		Data.TargetArmLength,
		InLerpAlpha);
}

void UGearSpecialAction::InterpToStartControlRotation(
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


const FCameraSequenceData* UGearSpecialAction::GetPreviousValidCameraData(int32 DataIndex) const
{
	if (!CameraSequenceData.IsValidIndex(DataIndex - 1))
	{
		return nullptr;
	}

	const FCameraSequenceData& PrevData = CameraSequenceData[DataIndex - 1];

	if (!PrevData.bPause)
	{
		return &PrevData;
	}

	if (!CameraSequenceData.IsValidIndex(DataIndex - 2))
	{
		return nullptr;
	}

	return &CameraSequenceData[DataIndex - 2];
}


int32 UGearSpecialAction::GetCurrentSectionIndex(float InElapsedTime)
{
	float ElapsedTime = InElapsedTime;

	// 経過時間から区間時間を引いていき、どの区間か調べる
	for (int32 DataIndex = 0; DataIndex < CameraSequenceData.Num(); ++DataIndex) {

		ElapsedTime -= CameraSequenceData[DataIndex].Time;
		if (ElapsedTime <= 0.f) {
			return DataIndex;
		}
	}

	return -1;
}

float UGearSpecialAction::GetElapsedTimeToIndex(int32 InTargetIndex)
{
	float ResultTime = 0.f;

	// TargetIndex以前の区間時間を加算して区間内での開始時間を返す
	for (int32 DataIndex = 0; DataIndex < CameraSequenceData.Num(); ++DataIndex)
	{
		if (DataIndex == InTargetIndex) { return ResultTime; }

		ResultTime += CameraSequenceData[DataIndex].Time;
	}

	return ResultTime;
}