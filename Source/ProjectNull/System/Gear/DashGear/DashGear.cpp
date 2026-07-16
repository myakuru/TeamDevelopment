#include "DashGear.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Combat/Attack/CollisionAttack/CollisionAttack.h>


UDashGear::UDashGear()
	:	SphereAttacks(TArray<TObjectPtr<UCollisionAttack>>())
	,	CurrentExecuteAttackIndex(0)
{
}

void UDashGear::Initialize(
	APlayerBase* Player,
	UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(
		Player,
		GearComponent);

	if (!IsValid(OwnerPlayer)) { return; }

	if (SphereAttacks.IsEmpty()) { return; }
	for (const auto& Attack : SphereAttacks)
	{
		if (!IsValid(Attack)) { continue; }
		Attack->Initialize(OwnerPlayer);
	}
}

void UDashGear::Execute(int32 CurrentGearLevel)
{
	UGearBase::Execute(CurrentGearLevel);
	
	// 発動したいギアステートに該当する攻撃のインデックス
	CurrentExecuteAttackIndex = CurrentGearLevel - 1;
	
	// 攻撃有効化
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	SphereAttacks[CurrentExecuteAttackIndex]->Execute();
}

void UDashGear::Update(float DeltaTime)
{
	UGearBase::Update(DeltaTime);

	if (!IsActive())
	{
		SetBlocksMovement(false);
	}
	
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	SphereAttacks[CurrentExecuteAttackIndex]->Update(DeltaTime);
}

void UDashGear::SetSphereTransform(const FTransform& Transform) const
{
	// コリジョンのトランスフォームを指定の値を基に変化させる
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	UE_LOG(LogTemp,Warning,TEXT("SetActorLocation!!"));
	SphereAttacks[CurrentExecuteAttackIndex]->ApplyCollisionTransform(Transform);
}
