#include "EnemyAttackComponent.h"

#include <Kismet/GameplayStatics.h>
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
	
	// 初期化
	for (auto& attack : EnemyAttacks)
	{
		if (!attack) { continue; }
		attack->Initialize(GetOwner());
	}
}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	for (auto& attack : EnemyAttacks)
	{
		if (!attack) { continue; }

		if (attack->CanExecute())
		{
			attack->Execute();
		}

		attack->Update(DeltaTime);
	}
}

bool UEnemyAttackComponent::IsAllAttackDeactivate()
{
	bool bIsAllDeactive = true;
	if (EnemyAttacks.IsEmpty()) { return bIsAllDeactive; }

	for (auto& Attack : EnemyAttacks)
	{
		if (!Attack) { continue; }
		if (Attack->IsActive())
		{
			bIsAllDeactive = false;
			break;
		}
	}

	return bIsAllDeactive;
}

void UEnemyAttackComponent::TestActive()
{
	if (EnemyAttacks.IsEmpty()) { return; }

	// 試しに0番目の要素をアクティブ化
	EnemyAttacks[0]->Execute();
}