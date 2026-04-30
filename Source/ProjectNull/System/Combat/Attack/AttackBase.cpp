#include "AttackBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UAttackBase::UAttackBase():
	OwnerActor(nullptr),
	bCanExecute(true),
	bIsActive(false)
{
}

void UAttackBase::Initialize(AActor* Owner)
{
	OwnerActor = Owner;

	RootComponent = NewObject<USceneComponent>(Owner);

	if (!RootComponent) { return; }

	RootComponent->RegisterComponent();
	RootComponent->AttachToComponent(
		Owner->GetRootComponent(),
		FAttachmentTransformRules::KeepRelativeTransform);
}

void UAttackBase::AttackJudge(AActor* Player, UEnemyManagerSubsystem* EnemyManager)
{
	if (Player)
	{
		AttackJudgePlayer(Player);
	}
	else if (EnemyManager)
	{
		AttackJudgeEnemys(EnemyManager);
	}
}

FVector UAttackBase::CalcAttackDir(const FVector& forwardVector) const
{
	return forwardVector.RotateAngleAxis(0.0f, FVector::UpVector);
}
