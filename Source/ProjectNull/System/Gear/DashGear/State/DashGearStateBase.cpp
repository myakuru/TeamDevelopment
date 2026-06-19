
#include "DashGearStateBase.h"

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
	DashAttackRangeSquared(30000.0f),
	DashSpeed(2000.0f),
	DashEffectDuration(0.3f)
{
}

void UDashGearStateBase::Execute(int32 CurrentGearLevel)
{
	UGearStateBase::Execute(CurrentGearLevel);

	

	PlayDashEffect();

	if (!Player) { return; }
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

	PlayerAnimInstance->Montage_Stop(0.2f);

	auto DashGear = Cast<UDashGear>(Owner);
	if (!DashGear) { return; }

	DashGear->SetSphereCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UDashGearStateBase::Dash()
{
	if (!Player)				{ return; }

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp)	{ return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp)				{ return; }

	//const FVector Dir = Player->GetActorForwardVector();
	const FVector Dir = RootComp->GetForwardVector();
	Player->LaunchCharacter(Dir * DashSpeed, true, true);

	if (Owner) {
		Owner->SetBlocksMovement(true);
	}

	UpdateDashAttack();
}

void UDashGearStateBase::PlayDashEffect()
{
	UNiagaraComponent* NiagaraComp = nullptr;
	if (!Player) { return; }

	if (DashEffect)
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			DashEffect,
			Player->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}


	if (NiagaraComp)
	{
		NiagaraComp->SetAutoDestroy(true);

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(
			TimerHandle,
			[NiagaraComp]()
			{
				if (NiagaraComp)
				{
					NiagaraComp->Deactivate();
				}
			},
			DashEffectDuration,
			false
		);
	}
}

void UDashGearStateBase::UpdateDashAttack()
{
	if (!Player) { return; }

	
}


