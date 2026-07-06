
#include "LaserGearState_Lv4.h"

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

#include <GameFramework/CharacterMovementComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Projectile/Laserbeam/Laserbeam.h>

#include <ProjectNull/System/Gear/DashGear/DashGear.h>
#include <ProjectNull/System/Gear/State/GearSpecialActionState/GearSpecialActionState.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/System/Controller/RobotController/RobotController.h>


ULaserGearState_Lv4::ULaserGearState_Lv4():
	SpellAnimMontage(nullptr),
	SpellAnimBlendOutTime(0.2f),
	bSpawnEnable(false)
{
	
}

void ULaserGearState_Lv4::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	ULaserGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);
	Laserbeam = GetWorld()->SpawnActor<ALaserbeam>(LaserbeamClass);
	if (!Laserbeam) { return; }

	if (!InPlayer) { return; }

	const auto GroundAlignmentComp = InPlayer->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	const auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp) { return; }

	Laserbeam->AttachToComponent(
		RootComp,
		FAttachmentTransformRules::KeepRelativeTransform);
	Laserbeam->SetLaserEnabled(false);

	if (!Owner) { return; }

	if (!GearSpecialAction) { return; }

	Owner->SetGearDuration(
		GearSpecialAction->GetTotalDuration(),
		GetGearLevelIndex()
	);

	GearSpecialAction->Initialize(this);
	
	RobotController = Cast<ARobotController>(InPlayer->GetController());
}

void ULaserGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);
	bSpawnEnable = false;
	
	if (!Player || 
		!RobotController) { return; }
	
	// 入力を無効化
	RobotController->SetCanReceiveInput(false);
	
	const auto MovementComp = Player->GetCharacterMovement(); 
	if (!MovementComp) { return;}
	
	MovementComp->bOrientRotationToMovement = false;
	MovementComp->SetMovementMode(MOVE_Flying);
	
	const auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(SpellAnimMontage);

	
	const FTransform& PlayerTransform = Player->GetActorTransform();
	
	if (!GearSpecialAction) { return; }
	GearSpecialAction->Execute(PlayerTransform);
	
	StartTransform = PlayerTransform; 
}

void ULaserGearState_Lv4::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);

	if (!Owner ||
		!Player ||
		!Laserbeam) { return; }
	
	// 経過時間取得
	const float ElapsedTime = Owner->GetElapsedTime();
	
	float LerpTime =  ElapsedTime < FlyingTime
	? ElapsedTime / FlyingTime
	: 1.f - (ElapsedTime - Owner->GetGearDuration(GetGearLevelIndex()) + WalkingTime) /  WalkingTime;
	
	const float LerpAlpha = FMath::Clamp(
			LerpTime,
			0.f,
			1.f);
	
	FVector ResultLocation = StartTransform.GetLocation();
	
	ResultLocation.Z =FMath::Lerp(
		StartTransform.GetLocation().Z,
		StartTransform.GetLocation().Z + TargetLocationOffsetZ,
		LerpAlpha);
	
	Player->SetActorLocation(ResultLocation);
	
	if (SpawnLaserThresholdRange.MinThreshold <= ElapsedTime)
	{
		if (!bSpawnEnable)
		{
			Laserbeam->SetLaserEnabled(true);
			bSpawnEnable = true;
		}
	}
	
	if (SpawnLaserThresholdRange.MaxThreshold <= ElapsedTime)
	{
		Laserbeam->SetLaserEnabled(false);
	}
	
	if (!GearSpecialAction) { return; }
	GearSpecialAction->Update(DeltaTime, ElapsedTime);
	
	UpdateRotation(DeltaTime,ElapsedTime);
}

void ULaserGearState_Lv4::End()
{
	ULaserGearStateBase::End();

	if (!Player) { return; }

	RobotController->SetCanReceiveInput(true);
	
	const auto MovementComp = Player->GetCharacterMovement(); 
	if (!MovementComp) { return;}
	
	MovementComp->bOrientRotationToMovement = true;
	MovementComp->SetMovementMode(MOVE_Walking);
	
	const auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(SpellAnimBlendOutTime);

	if (!Laserbeam) { return; }
	Laserbeam->SetLaserEnabled(false);

}

void ULaserGearState_Lv4::UpdateRotation(
	float InDeltaTime,
	float InElapsedTime)
{
	if (!Player) { return; }
	
	const int32 CurrentIndex = GetCurrentSectionIndex(InElapsedTime);
	
	FRotator TargetRotator = StartTransform.Rotator();
	
	if (!RotationYaws.IsValidIndex(CurrentIndex)) { return; }
	
	TargetRotator.Yaw += RotationYaws[CurrentIndex].TargetYawOffset;
	
	// 区間内での開始時間
	const float SectionStartTime = GetElapsedTimeToIndex(CurrentIndex);
	
	// 補間値を求める
	const float LerpAlpha = FMath::Clamp(
		(InElapsedTime - SectionStartTime) / RotationYaws[CurrentIndex].Time,
		0.f,
		1.f);
	//UE_LOG(LogTemp, Display, TEXT("hi LerpAlpha %.2f"), LerpAlpha);
	const FQuat ResultQuat = FQuat::Slerp(
		StartTransform.Rotator().Quaternion(),
		TargetRotator.Quaternion(),
		LerpAlpha) ;
	
	Player->SetActorRotation(ResultQuat);
}

int32 ULaserGearState_Lv4::GetCurrentSectionIndex(float InElapsedTime)
{
	float ElapsedTime = InElapsedTime;

	// 経過時間から区間時間を引いていき、どの区間か調べる
	for (int32 DataIndex = 0; DataIndex < RotationYaws.Num(); ++DataIndex) {

		ElapsedTime -= RotationYaws[DataIndex].Time;
		if (ElapsedTime <= 0.f) {
			return DataIndex;
		}
	}

	return -1;
}

float ULaserGearState_Lv4::GetElapsedTimeToIndex(int32 InTargetIndex)
{
	float ResultTime = 0.f;

	// TargetIndex以前の区間時間を加算して区間内での開始時間を返す
	for (int32 DataIndex = 0; DataIndex < RotationYaws.Num(); ++DataIndex)
	{
		if (DataIndex == InTargetIndex) { return ResultTime; }

		ResultTime += RotationYaws[DataIndex].Time;
	}

	return ResultTime;
}
