#include "EnemyAttackComponent.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>
#include <Kismet/GameplayStatics.h>

UEnemyAttackComponent::UEnemyAttackComponent():
	OwnerEnemy(nullptr),
	EnemyAttacks(TArray<UAttackBase*>())
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	
	// 配列内の攻撃クラスの初期化
	for (auto& attack : EnemyAttacks)
	{
		if (!attack) { continue; }
		attack->Initialize(GetOwner());
	}
}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// プレイヤーの情報取得
	// プレイヤーの情報を取得する（0番:1P）
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	// 配列内の攻撃クラスの更新
	for (auto& attack : EnemyAttacks)
	{
		if (!attack) { continue; }

		if (attack->CanExecute())
		{
			attack->Execute();
		}

		attack->Update(DeltaTime, pPlayerPawn);
	}
}

