
#include "DashGearState_Lv4.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

UDashGearState_Lv4::UDashGearState_Lv4()
{
}

void UDashGearState_Lv4::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UDashGearStateBase::Initialize(Player, GearComponent, Gear);

	if (!Player || !Gear) { return; }
	StartPlayerTransform = Player->GetTransform();

	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Initialize();
	AfterImageAttackEffect->Start(Player->GetTransform());

	auto* Camera = OwnerPlayer->GetSpringArmComponent();
	if (!Camera) { return; }

	StartTargetArmLength = Camera->TargetArmLength;

	if (auto* Controller = OwnerPlayer->GetController()) {
		StartControlRotation = Controller->GetControlRotation();
	}


	InitializeGearDuration();

	RobotController = Cast<ARobotController>(OwnerPlayer->GetController());
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!OwnerPlayer || !RobotController) { return; }

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
	const float ElapsedTime = OwnerGear->GetElapsedTime();

	UpdateCombatStance(ElapsedTime);

	AfterImageAttackEffect->Update(DeltaTime, ElapsedTime);

	UpdateCamera(DeltaTime);
	UpdateFinalDash(DeltaTime,ElapsedTime);
}

void UDashGearState_Lv4::End()
{
	if (!OwnerPlayer || !RobotController) { return; }
	RobotController->SetCanReceiveInput(true);

	OwnerPlayer->GetMesh()->SetHiddenInGame(false);
	
}

void UDashGearState_Lv4::UpdateCombatStance(float ElapsedTime)
{	
	if (!OwnerPlayer || !RobotController) { return; }

	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	if (!StanceTime.IsWithinRange(ElapsedTime)) {
		if (PlayerAnimInstance->bIsCombatStance) {
			PlayerAnimInstance->bIsCombatStance = false;
		}
		OwnerPlayer->GetMesh()->SetHiddenInGame(true);
	}
}

void UDashGearState_Lv4::UpdateCamera(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerGear || !RobotController) { return; }

	const int32 ResultIndex = GetCurrentSectionIndex(OwnerGear->GetElapsedTime());
	UE_LOG(LogTemp, Display, TEXT("hi ResultIndex %d"), ResultIndex);

	UpdateCameraRotation(DeltaTime, ResultIndex);

	UpdateTargetArmLength(DeltaTime,ResultIndex);
}

void UDashGearState_Lv4::UpdateCameraRotation(float DeltaTime, int32 DataIndex)
{
	if (!CameraData.IsValidIndex(DataIndex)
		|| !RobotController) { return; }
	FRotator	TargetRotator				= CameraData[DataIndex].TargetRotator;
	TargetRotator.Yaw						= CameraData[DataIndex].TargetRotator.Yaw + StartControlRotation.Yaw;
	const float		TargetRotatorLerpSpeed	= CameraData[DataIndex].RotatorLerpSpeed;
	const float		TargetArmLength			= CameraData[DataIndex].TargetArmLength;

	const FRotator Rotator = FMath::RInterpTo(
		RobotController->GetControlRotation(),
		TargetRotator,
		DeltaTime,
		TargetRotatorLerpSpeed);

	RobotController->SetControlRotation(Rotator);
}

void UDashGearState_Lv4::UpdateTargetArmLength(float DeltaTime, int32 DataIndex)
{
	if (!CameraData.IsValidIndex(DataIndex)
		|| !OwnerPlayer->GetSpringArmComponent()) { return; }

	auto* Camera = OwnerPlayer->GetSpringArmComponent();

	const float CurrentArmLength	= Camera->TargetArmLength;
	const float TargetArmLength		= CameraData[DataIndex].TargetArmLength;
	const float ArmLengthLerpSpeed	= CameraData[DataIndex].ArmLengthLerpSpeed;

	const float ResultArmLength = FMath::FInterpTo(
		CurrentArmLength, TargetArmLength, DeltaTime, ArmLengthLerpSpeed);

	Camera->TargetArmLength = ResultArmLength;
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

	for (auto& Data : CameraData) {
		TotalDuration += Data.Time;
	}

	OwnerGear->SetGearDuration(TotalDuration, kLv4Index);
}

int32 UDashGearState_Lv4::GetCurrentSectionIndex(float InElapsedTime)
{
	float ElapsedTime = InElapsedTime;

	for (int32 DataIndex = 0; DataIndex < CameraData.Num(); ++DataIndex) {

		ElapsedTime -= CameraData[DataIndex].Time;
		if (ElapsedTime <= 0.f) {
			return DataIndex;
		}
	}

	return 0;
}

