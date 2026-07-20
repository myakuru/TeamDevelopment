#include "AttackBase.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

UAttackBase::UAttackBase():
		OwnerActor(nullptr),
		bCanExecute(false),
		bIsActive(false),
		bAbsoluteScale(false),
		bAbsoluteRotation(false),
		bAbsoluteLocation(false)
{
}

void UAttackBase::Initialize(const TObjectPtr<AActor>& Owner)
{
	OwnerActor		= Owner;
	if (!OwnerActor.IsValid()) { return; }

	bIsActive=false;
	bCanExecute=false;
	
	RootComponent	= NewObject<USceneComponent>(OwnerActor.Get());
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

float UAttackBase::GetFinalDamage() const
{
	float OutFinalDamage = 0.0f;
	if (!GetOwnerActor().IsValid()){return OutFinalDamage;}
	
	// キャラクターインターフェースを実装しているか
	if (auto* Interface = Cast<ICharacterInterface>(OwnerActor))
	{
		// 「威力 + 最終的なオーナーの攻撃力」でダメージを決定
		OutFinalDamage = AttackPower + Interface->GetFinalAttackPower();
	}
	
	return OutFinalDamage;
}
