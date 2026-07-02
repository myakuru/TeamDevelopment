
#include "DashGearState_Lv4.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/System/Gear/DashGear/DashGear.h>

#include <ProjectNull/System/Gear/State/GearSpecialActionState/GearSpecialActionState.h>

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

UDashGearState_Lv4::UDashGearState_Lv4():
	RobotController(nullptr),
	AfterImageAttackEffect(nullptr),
	StanceTime(FThresholdRange()),
	DashTime(FThresholdRange()),
	CameraData(TArray<FCameraSequenceData>()),
	StartPlayerTransform(FTransform()),
	StanceAnimMontage(TObjectPtr<UAnimMontage>()),
	StanceAnimBlendOutTime(0.2f),
	bExecuteFinalDash(false),
	SpecialAttackSphereRadius(200.f)
{
}

void UDashGearState_Lv4::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	UDashGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);

	// ギア発動時間初期化
	InitializeGearDuration();

	// ================================================================
	// 残像攻撃エフェクトクラスの初期化
	// ================================================================
	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Initialize();

	if (!Player) { return; }
	// ロボットコントローラーの取得
	RobotController = Cast<ARobotController>(Player->GetController());
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!Player || 
		!RobotController ||
		!DashGear ||
		!GearSpecialAction) { return; }

	// ================================================================
	// ダッシュギアのレベル4状態クラスの初期化
	// ================================================================
	StartPlayerTransform	= Player->GetTransform();
	bExecuteFinalDash		= false;

	// ギアスキル発動前のカメラステータスを保存
	GearSpecialAction->SaveCameraStatus(Player);

	// 復元する際の開始データ初期化
	InitializeRestoreStartData();

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp) { return; }

	// 残像攻撃クラスの実行
	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Start(RootComp->GetComponentTransform());

	PlayStanceAnimation();

	// 入力を無効化
	RobotController->SetCanReceiveInput(false);

	DashGear->SetSphereRadius(SpecialAttackSphereRadius);

}

void UDashGearState_Lv4::Update(float DeltaTime)
{
	if (!Player || 
		!AfterImageAttackEffect) { return; }

	// 経過時間取得
	const float ElapsedTime = Owner->GetElapsedTime();

	// 攻撃用スフィア判定更新
	UpdateAttackSphereCollision(ElapsedTime);

	// 戦闘構え状態を更新
	UpdateCombatStance(ElapsedTime);

	// 残像エフェクトクラスの更新
	AfterImageAttackEffect->Update(DeltaTime, ElapsedTime);

	// カメラデータの更新
	UpdateCameraData(DeltaTime,ElapsedTime);

	// 最終ダッシュの更新処理
	UpdateFinalDash(DeltaTime,ElapsedTime);	 
}

void UDashGearState_Lv4::End()
{
	if (!Player || 
		!RobotController) { return; }

	EndDash();

	// 入力を有効化
	RobotController->SetCanReceiveInput(true);

	auto Mesh = Player->GetMesh();
	if (!Mesh) { return; }

	// プレイヤーのスケルタルメッシュ描画有効化
	Mesh->SetHiddenInGame(false);
}

void UDashGearState_Lv4::UpdateCombatStance(float ElapsedTime)
{	
	if (!Player)	{ return; }

	auto Mesh = Player->GetMesh();
	if (!Mesh)		{ return; }

	// 構え状態なら解除処理を行わない
	if (StanceTime.IsWithinRange(ElapsedTime))	{ return; }

	BlendOutStanceAnimation();

	// プレイヤーのスケルタルメッシュ描画無効化
	Mesh->SetHiddenInGame(true);
}

void UDashGearState_Lv4::UpdateAttackSphereCollision(float ElapsedTime)
{
	if(	!StanceTime.IsWithinRange(ElapsedTime) &&
		!DashTime.IsWithinRange(ElapsedTime)) { return; }

	SetSphereCollisionEnabled(ECollisionEnabled::QueryOnly);

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp) { return; }

	if (!DashGear) { return; }
	DashGear->SetSphereTransform(RootComp->GetComponentTransform());
}

void UDashGearState_Lv4::UpdateCameraData(
	float DeltaTime,
	float InElapsedTime)
{
	if (!GearSpecialAction) { return; }

	// カメラ復帰処理更新
	GearSpecialAction->UpdateCameraRestoreInterpolation(
		Player,
		Owner,
		GetGearLevelIndex(),
		DeltaTime);

	// 復元補間中は処理を行わない
	if (GearSpecialAction->GetCameraRestoreElapsedTime() >= 0.0f) { return; }

	// 現在の区間インデックス取得
	const int32 CurrentIndex = GetCurrentSectionIndex(InElapsedTime);

	if (!CameraData.IsValidIndex(CurrentIndex)) { return; }

	// 現在のカメラデータ取得
	const FCameraSequenceData& CurrentData = CameraData[CurrentIndex];

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

void UDashGearState_Lv4::UpdateCameraRotation(
	int32 DataIndex,
	float InLerpAlpha)
{
	if (!RobotController ||
		!GearSpecialAction)						{ return; }
	if (!CameraData.IsValidIndex(DataIndex))	{ return; }

	const FCameraSequenceData& Data = CameraData[DataIndex];

	if (Data.bPause)							{ return; }

	// ギアスキル発動開始時のプレイヤーYaw回転
	const float BaseYaw = StartPlayerTransform.GetRotation().Rotator().Yaw;

	// 区間開始の回転
	FRotator StartRotator = GearSpecialAction->StartControlRotation;

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

void UDashGearState_Lv4::UpdateTargetArmLength(
	int32 DataIndex,
	float InLerpAlpha)
{
	if (!Player ||
		!GearSpecialAction)						{ return; }

	auto SpringArm = Player->GetSpringArmComponent();
	if (!SpringArm)								{ return; }

	if (!CameraData.IsValidIndex(DataIndex))	{ return; }

	const FCameraSequenceData& Data = CameraData[DataIndex];

	if (Data.bPause)							{ return; }

	// 区間開始のカメラとプレイヤーの距離
	float StartArmLength = GearSpecialAction->StartTargetArmLength;

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

void UDashGearState_Lv4::UpdateFinalDash(
	float DeltaTime,
	float ElapsedTime)
{
	if (!Player)	{ return; }

	auto Mesh = Player->GetMesh();
	if (!Mesh)		{ return; }

	if (!DashTime.IsWithinRange(ElapsedTime)) { return; }

	if (!bExecuteFinalDash)
	{
		ExecuteDash();
	}

	bExecuteFinalDash = true;

	// プレイヤースケルタルメッシュの描画無効にする
	Mesh->SetHiddenInGame(false);

	// ダッシュ処理
	Dash();
}

const FCameraSequenceData* UDashGearState_Lv4::GetPreviousValidCameraData(int32 DataIndex) const
{
	if (!CameraData.IsValidIndex(DataIndex - 1))
	{
		return nullptr;
	}

	const FCameraSequenceData& PrevData = CameraData[DataIndex - 1];

	if (!PrevData.bPause)
	{
		return &PrevData;
	}

	if (!CameraData.IsValidIndex(DataIndex - 2))
	{
		return nullptr;
	}

	return &CameraData[DataIndex - 2];
}

void UDashGearState_Lv4::InitializeGearDuration()
{
	if (!Owner ||
		!GearSpecialAction) { return; }

	// ギア発動時間を更新
	Owner->SetGearDuration(GearSpecialAction->GetTotalDuration(), kLv4Index);
}

void UDashGearState_Lv4::InitializeRestoreStartData()
{
	if (!GearSpecialAction) { return; }

	// 最後の要素番号を取り出す
	const int32 MaxIndex = CameraData.Num() - 1;

	if (!CameraData.IsValidIndex(MaxIndex)) { return; }

	const auto& Data = CameraData[MaxIndex];

	// ギアスキル発動開始時のプレイヤーYaw回転
	const float BaseYaw = StartPlayerTransform.GetRotation().Rotator().Yaw;

	// 最後の要素データを取得
	GearSpecialAction->RestoreStartTargetArmLength = Data.TargetArmLength;
	GearSpecialAction->RestoreStartControlRotation = Data.TargetRotator;

	// オフセット考慮して計算
	GearSpecialAction->RestoreStartControlRotation.Yaw += BaseYaw;
}

int32 UDashGearState_Lv4::GetCurrentSectionIndex(float InElapsedTime)
{
	float ElapsedTime = InElapsedTime;

	// 経過時間から区間時間を引いていき、どの区間か調べる
	for (int32 DataIndex = 0; DataIndex < CameraData.Num(); ++DataIndex) {

		ElapsedTime -= CameraData[DataIndex].Time;
		if (ElapsedTime <= 0.f) {
			return DataIndex;
		}
	}

	return - 1;
}

float UDashGearState_Lv4::GetElapsedTimeToIndex(int32 InTargetIndex)
{
	float ResultTime = 0.f;

	// TargetIndex以前の区間時間を加算して区間内での開始時間を返す
	for (int32 DataIndex = 0; DataIndex < CameraData.Num(); ++DataIndex)
	{
		if (DataIndex == InTargetIndex) { return ResultTime; }

		ResultTime += CameraData[DataIndex].Time;
	}

	return ResultTime;
}

void UDashGearState_Lv4::PlayStanceAnimation()
{
	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(StanceAnimMontage);
}

void UDashGearState_Lv4::BlendOutStanceAnimation()
{
	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(StanceAnimBlendOutTime, StanceAnimMontage);
}

 