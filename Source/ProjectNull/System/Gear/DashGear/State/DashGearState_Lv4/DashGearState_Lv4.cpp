
#include "DashGearState_Lv4.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>
#include <ProjectNull/System/Gear/GearBase.h>

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


	Gear->SetGearDuration(TotalDuration, kLv4Index);

	RobotController = Cast<ARobotController>(OwnerPlayer->GetController());
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	if (!OwnerPlayer || !RobotController) { return; }

	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

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

void UDashGearState_Lv4::UpdateCombatStance(float ElapsedTime)
{	
	if (!OwnerPlayer || !RobotController) { return; }


	auto* PlayerAnimInstance = OwnerPlayer->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	if (ElapsedTime >= StanceMinTimeThreshold &&
		ElapsedTime < StanceMaxTimeThreshold) {
		
		/*SetMeshVisibility(false);
		SetMeshHiddenInGame(false);*/
		return;
	}
	else {
		PlayerAnimInstance->bIsCombatStance = false;
		RobotController->SetCanReceiveInput(true);
		/*SetMeshVisibility(true);
		SetMeshHiddenInGame(true);*/
	}
}

void UDashGearState_Lv4::UpdateCamera(float DeltaTime)
{
	if (!OwnerPlayer || !RobotController) { return; }
	float ElapsedTime = OwnerGear->GetElapsedTime();

	int32 ResultIndex = 0;
	float TotalTime = 0.f;

	for (int32 DataIndex = 0; DataIndex < CameraData.Num(); ++DataIndex) {

		ElapsedTime -= CameraData[DataIndex].Time;

		if (ElapsedTime <= 0.0f) {
			ResultIndex = DataIndex;
		}
	}

	if (!CameraData.IsValidIndex(ResultIndex)) { return; }
	const FRotator	TargetRotator	= CameraData[ResultIndex].TargetRotator;
	const float		LerpSpeed		= CameraData[ResultIndex].LerpSpeed;

	const FRotator Rotator = FMath::RInterpTo(
			RobotController->GetControlRotation(),
			TargetRotator,
			DeltaTime,
			LerpSpeed);

	RobotController->SetControlRotation(Rotator);
}

void UDashGearState_Lv4::SetMeshVisibility(bool bInVisibility) const
{
	if (!OwnerPlayer || !OwnerPlayer->GetMesh()) { return; }
	OwnerPlayer->GetMesh()->SetVisibility(bInVisibility);
}

void UDashGearState_Lv4::SetMeshHiddenInGame(bool bInHiddenInGame) const
{
	if (!OwnerPlayer || !OwnerPlayer->GetMesh()) { return; }
	OwnerPlayer->GetMesh()->SetHiddenInGame(bInHiddenInGame);

}
