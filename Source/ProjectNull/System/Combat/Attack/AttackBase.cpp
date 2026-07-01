#include "AttackBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UAttackBase::UAttackBase():
		OwnerActor(nullptr),
		bCanExecute(true),
		bIsActive(false),
		bAbsoluteScale(false),
		bAbsoluteRotation(false),
		bAbsoluteLocation(false)
{
}

void UAttackBase::Initialize(const TObjectPtr<AActor>& Owner)
{
	OwnerActor		= Owner;
	if (!OwnerActor) { return; }
	
	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OwnerActor))
	{
		// 最終的なダメージ量を算出
		FinalDamage = AttackPower + Interface->GetFinalAttackPower();
	}

	RootComponent	= NewObject<USceneComponent>(OwnerActor);
	if (!RootComponent) { return; }
	RootComponent->RegisterComponent();

	RootComponent->AttachToComponent(
			Owner->GetRootComponent(),
			FAttachmentTransformRules::KeepRelativeTransform
	);

	RootComponent->SetAbsolute(bAbsoluteLocation, bAbsoluteRotation, bAbsoluteScale);
}

FVector UAttackBase::CalcAttackDir(const FVector& ForwardVector) const
{
	return ForwardVector.RotateAngleAxis(0.f, FVector::UpVector);
}
