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

