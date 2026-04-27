#include "AttackBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UAttackBase::UAttackBase():
	OwnerActor(nullptr),
	bCanExecute(true),
	bIsActive(false),
	bIsDrawDebugLine(false)
{
}

void UAttackBase::Initialize(AActor* Owner)
{
	OwnerActor = Owner;
}

void UAttackBase::AttackJudge(AActor* Player, UEnemyManagerSubsystem* EnemyManager)
{
	if (Player)
	{
		// �v���C���[�ɑ΂���U�����菈��
		AttackJudgePlayer(Player);
	}
	else if (EnemyManager)
	{
		// �G���X�g�ɑ΂���U�����菈��
		AttackJudgeEnemys(EnemyManager);
	}
}

FVector UAttackBase::CalcAttackDir(const FVector& forwardVector) const
{
	return forwardVector.RotateAngleAxis(0.0f, FVector::UpVector);
}
