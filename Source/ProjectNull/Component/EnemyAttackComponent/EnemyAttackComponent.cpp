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
	
	// �z���̍U���N���X�̏�����
	for (auto& attack : EnemyAttacks)
	{
		if (!attack) { continue; }
		attack->Initialize(GetOwner());
	}
}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �v���C���[�̏��擾
	// �v���C���[�̏���擾����i0��:1P�j
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	// �z���̍U���N���X�̍X�V
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

