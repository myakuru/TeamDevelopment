#include "PlayerAttackComponent.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UPlayerAttackComponent::UPlayerAttackComponent()
	:	OwnerPlayer(nullptr)
	,	PlayerAttacks(TArray<UAttackBase*>())
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//　攻撃クラスの初期化
	for(auto& attack : PlayerAttacks)
	{
		if (!attack) { continue; }
		attack->Initialize(GetOwner());
	}	
}

void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// 敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//　攻撃クラスの更新
	for(auto& attack : PlayerAttacks)
	{
		if (!attack) { continue; }

		if (attack->CanExecute()) 
		{
			attack->Execute();
		}
		attack->Update(DeltaTime, nullptr, enemyManager);
	}
}

template<typename AttackType>
inline AttackType* UPlayerAttackComponent::AddAttack()
{
	AttackType* attack = NewObject<AttackType>(this);
	PlayerAttacks.Add(attack);
	return attack;
}

template<typename AttackType>
AttackType* UPlayerAttackComponent::FindAttack()
{
	for (UAttackBase* attack : PlayerAttacks)
	{
		if (AttackType* found = Cast<AttackType>(attack))
		{
			return found;
		}
	}
	return nullptr;
}

