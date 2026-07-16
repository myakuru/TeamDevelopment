
#include "LaserGearState_Lv4.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

#include <GameFramework/CharacterMovementComponent.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Projectile/Laserbeam/Laserbeam.h>

#include <ProjectNull/System/Gear/DashGear/DashGear.h>
#include <ProjectNull/System/Gear/State/GearSpecialActionState/GearSpecialActionState.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/System/Controller/RobotController/RobotController.h>

#include "ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h"
#include "ProjectNull/GameInstance/SuperGameInstance.h"


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

	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }

	PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
}

void ULaserGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);
	bSpawnEnable = false;
	
	if (!Player || 
		!RobotController ||
		!PlayerRuntimeData) { return; }

	//効果音
	if (GearSESound.IsValidIndex(SEIndex::BigLaserSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::LaserChargeSESoundIndex]);
	}
	
	//PlayerRuntimeData->SetIsInvincible(true);
	
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
	
	const float LerpTime =  ElapsedTime < FlyingTime
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
			
			//効果音
			if (GearSESound.IsValidIndex(SEIndex::BigLaserSESoundIndex))
			{
				GetWorld()->GetGameInstance<USuperGameInstance>()->
					GetSoundManager()->Play2D(GearSESound[SEIndex::BigLaserSESoundIndex]);
			}
			
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

	if (!Player ||
		!PlayerRuntimeData) { return; }

	//PlayerRuntimeData->SetIsInvincible(false);
	
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
	
	// 区間内での開始時間
	const float SectionStartTime = GetElapsedTimeToIndex(CurrentIndex);

	if (!RotationYaws.IsValidIndex(CurrentIndex)) { return; }
	
	// 補間値を求める
	const float LerpAlpha = FMath::Clamp(
		(InElapsedTime - SectionStartTime) / RotationYaws[CurrentIndex].Time,
		0.f,
		1.f);
	//UE_LOG(LogTemp, Display, TEXT("hi LerpAlpha %.2f"), LerpAlpha);
	
	// 区間開始のカメラとプレイヤーの距離
	FRotator StartRotator = StartTransform.Rotator();
	FRotator TargetRotator = StartTransform.Rotator();
	
	if (!RotationYaws.IsValidIndex(CurrentIndex)) { return; }
	
	TargetRotator.Yaw += RotationYaws[CurrentIndex].TargetYawOffset;
	
	// 前区間のカメラデータ取得
	if (const FRotationYaw* PrevData = GetPreviousValidRotationYawData(CurrentIndex))
	{
		StartRotator.Yaw += PrevData->TargetYawOffset;
	}
	
	const FQuat ResultQuat = FQuat::Slerp(
		StartRotator.Quaternion(),
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

const FRotationYaw* ULaserGearState_Lv4::GetPreviousValidRotationYawData(int32 DataIndex) const
{
	if (!RotationYaws.IsValidIndex(DataIndex - 1))
	{
		return nullptr;
	}

	const FRotationYaw& PrevData = RotationYaws[DataIndex - 1];
	
	return &PrevData;
}

