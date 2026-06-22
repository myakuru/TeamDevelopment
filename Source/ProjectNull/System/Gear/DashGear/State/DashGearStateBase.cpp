
#include "DashGearStateBase.h"



#include <ProjectNull/Actor/Effect/EffectBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Gear/DashGear/DashGear.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>


#include <ProjectNull/Utility/GroundUtility/GroundUtility.h>

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UDashGearStateBase::UDashGearStateBase():
	DashDir(FVector::ZeroVector),
	StartQuat(FQuat::Identity),
	DashSpeed(2000.0f),
	DashEffectDuration(0.3f),
	MontageBlendOutTime(0.2f)
{
}

void UDashGearStateBase::Execute(int32 CurrentGearLevel)
{
	UGearStateBase::Execute(CurrentGearLevel);

	if (!Player)				{ return; }

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp)	{ return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp)				{ return; }

	DashDir = RootComp->GetForwardVector();
	StartQuat = RootComp->GetComponentQuat();

	if (DashEffect) {
		DashEffect->Start(RootComp);
		// 位置だけ親に追従
		DashEffect->SetAbsolute(false, true, true);
	}

	auto EffectComp = DashEffect->GetEffectComponent();
	if (!EffectComp) { return; }

	EffectComp->SetWorldRotation(StartQuat);

	PlayDashEffect();

	auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(Player->GetPlayerAnimInstance());
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(DashAnimMontage);

	auto DashGear = Cast<UDashGear>(Owner);
	if (!DashGear) { return; }

	DashGear->SetSphereCollisionEnabled(ECollisionEnabled::PhysicsOnly);

}

void UDashGearStateBase::Update(float DeltaTime)
{
	if (!Owner || !Player) { return; }

	Dash();
}

void UDashGearStateBase::End()
{
	if (!Player) { return; }

	auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(Player->GetPlayerAnimInstance());
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(MontageBlendOutTime);

	if (DashEffect) {
		DashEffect->DeactivateEffect();
	}

	auto DashGear = Cast<UDashGear>(Owner);
	if (!DashGear) { return; }

	DashGear->SetSphereCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDashGearStateBase::Dash()
{
	if (!Player)				{ return; }

	Player->LaunchCharacter(DashDir * DashSpeed, true, true);
	//UE_LOG(LogTemp, Display, TEXT("DashDir X%.2f Y%.2f Z%.2f"), DashDir.X,DashDir.Y,DashDir.Z);

	if (Owner) {
		Owner->SetBlocksMovement(true);
	}

	UpdateDashAttack();
}

void UDashGearStateBase::PlayDashEffect()
{
	if (!Player ||
		!DashEffect) { return; }

	auto EffectComp = DashEffect->GetEffectComponent();
	if (!EffectComp) { return; }

	EffectComp->SetWorldRotation(StartQuat);

}

void UDashGearStateBase::UpdateDashAttack()
{
	if (!Player) { return; }

	
}


