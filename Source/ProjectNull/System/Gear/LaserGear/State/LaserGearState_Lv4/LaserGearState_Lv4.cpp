
#include "LaserGearState_Lv4.h"

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

#include <ProjectNull/Actor/Effect/EffectBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Projectile/Laserbeam/Laserbeam.h>

#include <ProjectNull/System/Gear/DashGear/DashGear.h>
#include <ProjectNull/System/Gear/State/GearSpecialActionState/GearSpecialActionState.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

ULaserGearState_Lv4::ULaserGearState_Lv4():
	SpellAnimMontage(nullptr),
	SpellAnimBlendOutTime(0.2f)
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

	auto GroundAlignmentComp = InPlayer->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
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
}

void ULaserGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);

	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(SpellAnimMontage);

	if (!Laserbeam) { return; }
	Laserbeam->SetLaserEnabled(true);

	if (!GearSpecialAction) { return; }

	GearSpecialAction->Execute(Player->GetActorTransform());
}

void ULaserGearState_Lv4::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);

	if (!Owner) { return; }

	// 経過時間取得
	const float ElapsedTime = Owner->GetElapsedTime();

	if (!GearSpecialAction) { return; }
	GearSpecialAction->Update(DeltaTime, ElapsedTime);

}

void ULaserGearState_Lv4::End()
{
	ULaserGearStateBase::End();

	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(SpellAnimBlendOutTime);

	if (!Laserbeam) { return; }
	Laserbeam->SetLaserEnabled(false);

}
