
#include "LaserGearState_Lv4.h"

#include <ProjectNull/Actor/Effect/EffectBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>


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
}

void ULaserGearState_Lv4::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);

	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(SpellAnimMontage);

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp) { return; }

	if (Laserbeam) { Laserbeam->Start(RootComp); }
}

void ULaserGearState_Lv4::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);
		
}

void ULaserGearState_Lv4::End()
{
	ULaserGearStateBase::End();
	if (Laserbeam) { Laserbeam->DeactivateEffect(); }

	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(SpellAnimBlendOutTime);

}
