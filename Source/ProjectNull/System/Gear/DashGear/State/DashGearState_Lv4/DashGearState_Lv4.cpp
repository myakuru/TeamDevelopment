
#include "DashGearState_Lv4.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>
#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

UDashGearState_Lv4::UDashGearState_Lv4():
	StanceMinTimeThreshold(0.0f),
	StanceMaxTimeThreshold(0.0f)
{
}

void UDashGearState_Lv4::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UDashGearStateBase::Initialize(Player, GearComponent, Gear);

	if (!Player || !Gear) { return; }
	StartPlayerTransform = Player->GetTransform();

	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Initialize();
	//const float TotalDuration = AfterImageAttackEffect->GetMaxTime();
	float TotalDuration = 0.f;

	for (auto& Data : CameraData) {
		TotalDuration += Data.Time;
	}

	auto* Camera = OwnerPlayer->GetSpringArmComponent();
	if (!Camera) { return; }

	StartTargetArmLength = Camera->TargetArmLength;

	Gear->SetGearDuration(TotalDuration, kLv4Index);

	RobotController = Cast<ARobotController>(OwnerPlayer->GetController());
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!OwnerPlayer || !RobotController) { return; }

	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	// アニメーションを構え状態にする
	PlayerAnimInstance->bIsCombatStance = true;

	RobotController->SetCanReceiveInput(false);

}

void UDashGearState_Lv4::Update(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerGear || !AfterImageAttackEffect) { return; }
	const float ElapsedTime = OwnerGear->GetElapsedTime();

	UpdateCombatStance(ElapsedTime);

	AfterImageAttackEffect->Update(DeltaTime, ElapsedTime, StartPlayerTransform);

	UpdateCamera(DeltaTime);
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

	if (ElapsedTime > StanceMaxTimeThreshold) {
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
	
	UpdateCameraRotation(DeltaTime, ResultIndex);

	UpdateTargetArmLength(DeltaTime,ResultIndex);
}

void UDashGearState_Lv4::UpdateCameraRotation(float DeltaTime, int32 DataIndex)
{
	if (!CameraData.IsValidIndex(DataIndex)
		|| !RobotController) { return; }
	const FRotator	TargetRotator			= CameraData[DataIndex].TargetRotator;
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

