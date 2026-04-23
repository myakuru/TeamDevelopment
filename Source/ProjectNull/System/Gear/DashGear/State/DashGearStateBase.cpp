
#include "DashGearStateBase.h"

#include "ProjectNull/System/Gear/GearBase.h"
#include "ProjectNull/Actor/Character/Player/PlayerBase.h"
#include "ProjectNull/Actor/Character/Enemy/EnemyBase.h"
#include "ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h"
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
}

void UDashGearStateBase::Update(float DeltaTime)
{
	if (!OwnerGear || !OwnerPlayer) { return; }

	Dash();
}

void UDashGearStateBase::Dash()
{
	if (!OwnerPlayer) { return; }

	const FVector dir = OwnerPlayer->GetActorForwardVector();
	OwnerPlayer->LaunchCharacter(dir * DashSpeed, true, true);

	if (OwnerGear)
	{
		OwnerGear->SetBlocksMovement(true);
	}



	UpdateDashAttack();
}

void UDashGearStateBase::PlayDashEffect()
{
	UNiagaraComponent* NiagaraComp = nullptr;

	if (DashEffect)
	{
		NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(
			DashEffect,
			OwnerPlayer->GetRootComponent(),
			NAME_None,
			FVector::ZeroVector,
			FRotator::ZeroRotator,
			EAttachLocation::KeepRelativeOffset,
			true
		);
	}


	if (NiagaraComp)
	{
		NiagaraComp->SetAutoDestroy(true); // I‚í‚Á‚½‚çŽ©“®íœ

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
	if (!OwnerPlayer) { return; }

	// ƒvƒŒƒCƒ„[‚ÌˆÊ’u‚ÌŽæ“¾
	const FVector playerLocation = OwnerPlayer->GetActorLocation();

	// “GŠÇ—ƒNƒ‰ƒX‚Ìî•ñŽæ“¾
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	// “G‚ÌƒŠƒXƒg‚©‚ç‹——£‚ð”äŠr‚µ‚ÄUŒ‚”ÍˆÍ“à‚Ì“G‚ðUŒ‚
	for (const auto& enemy : enemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		const float DistSq = FVector::DistSquared(playerLocation, enemy->GetActorLocation());

		if (DistSq <= DashAttackRangeSquared)
		{
			enemy->SetKnockBackData(playerLocation,3,1);
			enemy->SetTakeDamaged();
		}
	}

	// UŒ‚”ÍˆÍ‚ÌƒfƒoƒbƒO•\Ž¦
	DrawDebugSphere(
		GetWorld(),
		OwnerPlayer->GetActorLocation(),
		FMath::Sqrt(DashAttackRangeSquared),
		16,
		FColor::Green,
		false,
		0.1f
	);
}


