
#include "DashGearState_Lv4.h"

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

	if (!GearSpecialAction) { return; }
	GearSpecialAction->Initialize(this);
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
	bExecuteFinalDash = false;

	// ギアスキル発動前のカメラステータスを保存
	GearSpecialAction->Execute(Player->GetTransform());

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
	if (!Player) { return; }

	// 経過時間取得
	const float ElapsedTime = Owner->GetElapsedTime();

	// 攻撃用スフィア判定更新
	UpdateAttackSphereCollision(ElapsedTime);

	// 戦闘構え状態を更新
	UpdateCombatStance(ElapsedTime);

	if (!AfterImageAttackEffect) { return; }

	// 残像エフェクトクラスの更新
	AfterImageAttackEffect->Update(DeltaTime, ElapsedTime);

	// カメラデータの更新
	if (!GearSpecialAction) { return; }
	GearSpecialAction->Update(DeltaTime, ElapsedTime);

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

	const auto GroundAlignmentRootComp = Player->GetGroundAlignmentRootComponent();
	if (!GroundAlignmentRootComp) { return; }

	if (!DashGear) { return; }
	DashGear->SetSphereTransform(GroundAlignmentRootComp->GetComponentTransform());
}

void UDashGearState_Lv4::UpdateFinalDash(
	float DeltaTime,
	float ElapsedTime)
{
	if (!Player)	{ return; }

	const auto Mesh = Player->GetMesh();
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

void UDashGearState_Lv4::InitializeGearDuration()
{
	if (!Owner ||
		!GearSpecialAction) { return; }

	// ギア発動時間を更新
	Owner->SetGearDuration(GearSpecialAction->GetTotalDuration(), kLv4Index);
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

 