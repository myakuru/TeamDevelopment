
#include "DashGearState_Lv4.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

UDashGearState_Lv4::UDashGearState_Lv4():
	RobotController(nullptr),
	AfterImageAttackEffect(nullptr),
	StanceTime(FThresholdRange()),
	DashTime(FThresholdRange()),
	CameraData(TArray<FCameraSequenceData>()),
	StartPlayerTransform(FTransform()),
	StartControlRotation(FRotator::ZeroRotator),
	StartTargetArmLength(0.f)
{
}

void UDashGearState_Lv4::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UDashGearStateBase::Initialize(Player, GearComponent, Gear);

	// ギア発動時間初期化
	InitializeGearDuration();

	// ================================================================
	// 残像攻撃エフェクトクラスの初期化
	// ================================================================
	if (!AfterImageAttackEffect)	{ return; }
	AfterImageAttackEffect->Initialize();

	if (!Player)					{ return; }
	// ロボットコントローラーの取得
	RobotController = Cast<ARobotController>(Player->GetController());
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!OwnerPlayer || !RobotController) { return; }

	auto* Camera = OwnerPlayer->GetSpringArmComponent();
	if (!Camera) { return; }

	auto* Controller = OwnerPlayer->GetController();
	if (!Controller) { return; }

	// ================================================================
	// ダッシュギアのレベル4状態クラスの初期化
	// ================================================================
	StartPlayerTransform = OwnerPlayer->GetTransform();
	StartTargetArmLength = Camera->TargetArmLength;
	StartControlRotation = Controller->GetControlRotation();

	
	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Start(OwnerPlayer->GetTransform());

	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	// アニメーションを構え状態にする
	PlayerAnimInstance->bIsCombatStance = true;

	// 入力を無効化
	RobotController->SetCanReceiveInput(false);
}

void UDashGearState_Lv4::Update(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerGear || !AfterImageAttackEffect) { return; }

	// 経過時間取得
	const float ElapsedTime = OwnerGear->GetElapsedTime();

	// 戦闘構え状態を更新
	UpdateCombatStance(ElapsedTime);

	// 残像エフェクトクラスの更新
	AfterImageAttackEffect->Update(DeltaTime, ElapsedTime);

	// カメラデータの更新
	UpdateCameraData(DeltaTime);

	// 最終ダッシュの更新処理
	UpdateFinalDash(DeltaTime,ElapsedTime);
}

void UDashGearState_Lv4::End()
{
	if (!OwnerPlayer || !RobotController) { return; }

	// 入力を有効化
	RobotController->SetCanReceiveInput(true);

	// プレイヤーのスケルタルメッシュ描画有効化
	OwnerPlayer->GetMesh()->SetHiddenInGame(false);
}

void UDashGearState_Lv4::UpdateCombatStance(float ElapsedTime)
{	
	if (!OwnerPlayer || !OwnerPlayer->GetMesh() || !RobotController)	{ return; }

	// 構え状態なら解除処理を行わない
	if (StanceTime.IsWithinRange(ElapsedTime))							{ return; }

	// プレイヤーのアニメーションインスタンス取得
	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	// 構え状態を解除
	PlayerAnimInstance->bIsCombatStance = false;

	// プレイヤーのスケルタルメッシュ描画無効化
	OwnerPlayer->GetMesh()->SetHiddenInGame(true);
}

void UDashGearState_Lv4::UpdateCameraData(float DeltaTime)
{
	if (!OwnerGear) { return; }

	const int32 ResultIndex = GetCurrentSectionIndex(OwnerGear->GetElapsedTime());

	UpdateCameraRotation(DeltaTime, ResultIndex);

	UpdateTargetArmLength(DeltaTime, ResultIndex);
}

void UDashGearState_Lv4::UpdateCameraRotation(float DeltaTime, int32 DataIndex)
{
	if (!RobotController)						{ return; }
	if (!CameraData.IsValidIndex(DataIndex))	{ return; }

	// 演出カメラ構造体からデータを取得
	FRotator		TargetRotator			= CameraData[DataIndex].TargetRotator;
	const float		TargetRotatorLerpSpeed	= CameraData[DataIndex].RotatorLerpSpeed;

	// カメラのYaw回転を考慮して計算
	TargetRotator.Yaw						= CameraData[DataIndex].TargetRotator.Yaw + StartControlRotation.Yaw;

	// 補間処理
	const FRotator Rotator = FMath::RInterpTo(	RobotController->GetControlRotation(),
												TargetRotator,
												DeltaTime,
												TargetRotatorLerpSpeed);

	// 計算結果を更新
	RobotController->SetControlRotation(Rotator);
}

void UDashGearState_Lv4::UpdateTargetArmLength(float DeltaTime, int32 DataIndex)
{
	auto* SpringArm = OwnerPlayer->GetSpringArmComponent();
	if (!SpringArm)								{ return; }
	if (!CameraData.IsValidIndex(DataIndex))	{ return; }

	// 現在のプレイヤーとカメラ距離
	const float CurrentArmLength	= SpringArm->TargetArmLength;

	// 演出カメラ構造体からデータを取得
	const float TargetArmLength		= CameraData[DataIndex].TargetArmLength;
	const float ArmLengthLerpSpeed	= CameraData[DataIndex].ArmLengthLerpSpeed;

	// 補間処理
	const float ResultArmLength = FMath::FInterpTo(	CurrentArmLength,
													TargetArmLength,
													DeltaTime,
													ArmLengthLerpSpeed);
	// 計算結果を更新
	SpringArm->TargetArmLength = ResultArmLength;
}

void UDashGearState_Lv4::UpdateFinalDash(float DeltaTime, float ElapsedTime)
{
	if (!DashTime.IsWithinRange(ElapsedTime) || !OwnerPlayer || !OwnerPlayer->GetMesh()) { return; }

	// プレイヤースケルタルメッシュの描画無効にする
	OwnerPlayer->GetMesh()->SetHiddenInGame(false);

	// ダッシュ処理
	Dash();
}

void UDashGearState_Lv4::InitializeGearDuration()
{
	if (!OwnerGear) { return; }

	float TotalDuration = 0.f;

	// 区間時間の合計時間をギア発動時間とする
	for (auto& Data : CameraData) {
		TotalDuration += Data.Time;
	}

	// ギア発動時間を更新
	OwnerGear->SetGearDuration(TotalDuration, kLv4Index);
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

	return 0;
}

