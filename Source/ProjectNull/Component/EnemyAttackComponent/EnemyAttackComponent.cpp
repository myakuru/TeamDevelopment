#include "EnemyAttackComponent.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>

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
	
	// プレイヤーの情報を渡して
	// 攻撃処理側は受け取ったプレイヤーアドレスを基に判定を行う
}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// 配列内の攻撃クラスの更新
}

