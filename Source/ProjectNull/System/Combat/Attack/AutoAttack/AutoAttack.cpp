#include "AutoAttack.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/Actor/Character/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h>
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>



UAutoAttack::UAutoAttack()
	: AutoAttackInterval(5.0f)
	, FrontToRingDelay(1.0f)
{
}

void UAutoAttack::Initialize(AActor* Owner)
{
	UAttackBase::Initialize(Owner);


	if (FloatingWeaponMap.Contains(EAutoAttackType::Front))
	{
		if (auto* floatingWeapon = FloatingWeaponMap[EAutoAttackType::Front])
		{
			floatingWeapon->SetOwnerAttack(AutoAttackParamsMap[EAutoAttackType::Front]);
			floatingWeapon->Start(OwnerActor->GetRootComponent());
		}
	}

	if (FloatingWeaponMap.Contains(EAutoAttackType::Ring))
	{
		if (auto* floatingWeapon = FloatingWeaponMap[EAutoAttackType::Ring])
		{
			floatingWeapon->SetOwnerAttack(AutoAttackParamsMap[EAutoAttackType::Ring]);
		}
	}


	for (auto& [type, floatingWeapon] : FloatingWeaponMap)
	{
		if (!floatingWeapon) { continue; }
		floatingWeapon->Initialize();
	}

	// �����U���̃^�C�}�[��Z�b�g
	GetWorld()->GetTimerManager().SetTimer(
		AutoFrontConeAttackTimerHandle,
		this,
		&UAutoAttack::StartAutoAttack,
		AutoAttackInterval,
		true);


	/*if (FloatingWeaponMap.Contains(EAutoAttackType::Front)
		&& FloatingWeaponMap[EAutoAttackType::Front])
	{
		FloatingWeaponMap[EAutoAttackType::Front]->Start(OwnerActor->GetRootComponent());
	}*/
}

void UAutoAttack::Execute()
{
	return;
}

void UAutoAttack::Update(float DeltaTime)
{
	// �G�Ǘ��N���X�̏��擾
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	// �����U���̍X�V
	for(auto& [type,coneSlashParams] : AutoAttackParamsMap)
	{
		if (!coneSlashParams) { continue; }

		UpdateAutoAttack(DeltaTime, *coneSlashParams, enemyManager);
	}

	// ���V����̍X�V
	for (auto& [type, floatingWeapon] : FloatingWeaponMap)
	{
		if (!floatingWeapon) { continue; }

		floatingWeapon->Update(OwnerActor,DeltaTime);
	}

}

void UAutoAttack::StartAutoAttack()
{
	if (!OwnerActor) { return; }

	if(AutoAttackParamsMap.Contains(EAutoAttackType::Front)
		&& AutoAttackParamsMap[EAutoAttackType::Front])
	{
		AutoAttackParamsMap[EAutoAttackType::Front]->Start();	
	}

	
	

	// �O����󎩓��U������̎��͍U���x���^�C�}�[��Z�b�g
	GetWorld()->GetTimerManager().SetTimer(
		FrontToRingDelayTimerHandle,
		this,
		&UAutoAttack::StartAutoRingAttack,
		FrontToRingDelay,
		false);
}

void UAutoAttack::StartAutoRingAttack()
{
	if (FloatingWeaponMap.Contains(EAutoAttackType::Ring)
		&& FloatingWeaponMap[EAutoAttackType::Ring])
	{
		FloatingWeaponMap[EAutoAttackType::Ring]->Start(OwnerActor->GetRootComponent());
	}
}

void UAutoAttack::Execute()
{
	return;
}

void UAutoAttack::Update(float DeltaTime,APlayerBase* Player,UEnemyManagerSubsystem* EnemyManager)
{
	if (!EnemyManager) { return; }

	for(auto& [type,ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }

		if (!ConeSlashParams->UpdateAttack(DeltaTime)) { continue; }

		ConeSlashParams->AttackJudge(Player, EnemyManager);

		//UpdateAutoAttack(DeltaTime, *ConeSlashParams, EnemyManager);
	}
}


void UAutoAttack::UpdateAutoAttack(float DeltaTime, UFanAttackBase& RingPulseSlashAttack, UEnemyManagerSubsystem* EnemyManager)
{
	//if (!OwnerActor) { return; }
	//if (!EnemyManager) { return; }

	//if (!RingPulseSlashAttack.UpdateAttack(DeltaTime)) { return; }

	////�@�v���C���[�̍��W�ƑO���x�N�g����擾
	//const FVector playerLocation	= OwnerActor->GetActorLocation();
	//const FVector forwardVector		= OwnerActor->GetActorForwardVector();

	////�@�U�������x�N�g��
	//const FVector attackDir = RingPulseSlashAttack.CalcAttackDir(forwardVector);

	//{
	//	//�@�U���͈͂�f�o�b�O���C���ŉ���
	//	UDebugDrawLibrary::DrawDebugFan(
	//		GetWorld(),
	//		playerLocation,
	//		attackDir,
	//		RingPulseSlashAttack.Radius,
	//		RingPulseSlashAttack.ConeAngle,
	//		10
	//	);
	//}

	////�@�G���X�g����[�v���āA�U���͈͓�̓G�Ƀ_���[�W��^����
	//for (auto& enemy : EnemyManager->GetEnemyList())
	//{
	//	if (!enemy) { continue; }

	//	//�@�G���U���͈͓�ɂ��邩����
	//	if (RingPulseSlashAttack.IsTargetInRange(enemy, playerLocation, attackDir))
	//	{
	//		enemy->SetKnockBackData(playerLocation, RingPulseSlashAttack.KnockbackPower,1.0f);

	//		// �_���[�W��^����()
	//		enemy->SetTakeDamaged(10);
	//	}
	//}
}
