#include "AttackBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UAttackBase::UAttackBase()
	:	OwnerActor(nullptr)
	,	bCanExecute(true)
	,	bIsActive(false)
{
}

void UAttackBase::Initialize(AActor* Owner)
{
	OwnerActor = Owner;
}

void UAttackBase::AttackJudge(APlayerBase* Player, UEnemyManagerSubsystem* EnemyManager)
{
	if (Player)
	{
		// プレイヤーに対する攻撃判定処理
		AttackJudgePlayer(Player);
	}
	else if (EnemyManager)
	{
		// 敵リストに対する攻撃判定処理
		AttackJudgeEnemys(EnemyManager);
	}
}

FVector UAttackBase::CalcAttackDir(const FVector& forwardVector) const
{
	return forwardVector.RotateAngleAxis(0.0f, FVector::UpVector);
}
