#include "EnemyAttackComponent.h"

#include <Kismet/GameplayStatics.h>
#include <ProjectNull/System/Combat/Attack/AttackBase.h>

UEnemyAttackComponent::UEnemyAttackComponent() :
	OwnerEnemy(nullptr),
	EnemyAttacks(TMap<EEnemyAttackType, TObjectPtr<UAttackBase>>())
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	// 初期化
	for (auto& Attack : EnemyAttacks)
	{
		if (!IsValid(Attack.Value)) { continue; }
		Attack.Value->Initialize(GetOwner());
		AttackTimerHandles.Add(Attack.Key, FTimerHandle());
	}
}


void UEnemyAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                          FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	for (auto& Attack : EnemyAttacks)
	{
		if (!IsValid(Attack.Value)) { continue; }

		if (Attack.Value->CanExecute())
		{
			Attack.Value->Execute(TargetLocation);
		}

		Attack.Value->Update(DeltaTime);
	}
}

void UEnemyAttackComponent::AllAtackDeactivate()
{
	if (EnemyAttacks.IsEmpty()) { return; }

	for (auto& Attack : EnemyAttacks)
	{
		if (!IsValid(Attack.Value)) { continue; }
		Attack.Value->Cancel();
	}
}

bool UEnemyAttackComponent::IsAllAttackDeactivate()
{
	bool bIsAllDeactive = true;
	if (EnemyAttacks.IsEmpty()) { return bIsAllDeactive; }

	for (auto& Attack : EnemyAttacks)
	{
		if (!IsValid(Attack.Value)) { continue; }
		if (Attack.Value->IsActive())
		{
			bIsAllDeactive = false;
			break;
		}
	}

	return bIsAllDeactive;
}

void UEnemyAttackComponent::AttackActive(EEnemyAttackType InAttackType)
{
	if (AttackTimerHandles.IsEmpty() ||
		!AttackTimerHandles.Find(InAttackType)) { return; }
	
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([this,InAttackType]
		{
			if (EnemyAttacks.IsEmpty() ||
				!AttackTimerHandles.Find(InAttackType)) { return; }

			// 攻撃が現在発動中なら有効化しない
			if (!EnemyAttacks[InAttackType]->IsActive())
			{
				EnemyAttacks[InAttackType]->SetCanExecute(true);
			}
		}
	);

	// タイマーは0秒だと無効と判断するので条件分岐
	const float Deray = EnemyAttacks[InAttackType]->GetAttackStartDelay();
	if (Deray <= 0.f)
	{
		if (EnemyAttacks.IsEmpty() ||
				!AttackTimerHandles.Find(InAttackType)) { return; }
		
		// 攻撃が現在発動中なら有効化しない
		if (!EnemyAttacks[InAttackType]->IsActive())
		{
			EnemyAttacks[InAttackType]->SetCanExecute(true);
		}
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(
			AttackTimerHandles[InAttackType],
			TimerDelegate,
			Deray,
			false
		);
	}
}

void UEnemyAttackComponent::TestActive()
{
	if (EnemyAttacks.IsEmpty()) { return; }

	// 試しに0番目の要素をアクティブ化
	EnemyAttacks[EEnemyAttackType::Attack1]->SetCanExecute(true);
}
