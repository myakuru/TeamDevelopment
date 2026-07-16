#include "PositionAttack.h"

#include <ProjectNull/System/Combat/Attack/CollisionAttack/SphereAttack/SphereAttack.h>

UPositionAttack::UPositionAttack()
{
}

void UPositionAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	UAttackBase::Initialize(InOwner);
	
	SphereAttack = NewObject<USphereAttack>(this,SubSphereAttack);
	if (!IsValid(SphereAttack)) { return; }
	SphereAttack->Initialize(GetOwnerActor().Get());
}

void UPositionAttack::Execute(const FVector& InTargetLocation)
{
	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	
	if (!IsValid(SphereAttack)){return;}
	SphereAttack->Execute();
	
	FTransform TargetTransform;
	TargetTransform.SetTranslation(InTargetLocation);
	SphereAttack->ApplyCollisionTransform(TargetTransform);
}

void UPositionAttack::Cancel()
{
	SetIsActive(false);
}

void UPositionAttack::Update(float InDeltaTime)
{
	if (!IsActive()) { return; }
	
	UAttackBase::Update(InDeltaTime);
	
	if (!IsValid(SphereAttack)) { return; }
	SphereAttack->Update(InDeltaTime);
	
	// 球が消滅していたら終了
	if (!SphereAttack->IsActive())
	{
		Cancel();
	}
}
