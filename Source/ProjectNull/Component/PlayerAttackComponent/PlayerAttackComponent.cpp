// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAttackComponent.h"



#include "../../System/Combat/Attack/AttackBase.h"
#include "../../System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h"

UPlayerAttackComponent::UPlayerAttackComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	
}


void UPlayerAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	//AddAttack<URingPulseSlashAttack>();

	for(auto& attack : PlayerAttacks)
	{
		if (!attack) { continue; }
		attack->Initialize(GetOwner());
	}


	
}


void UPlayerAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(auto& attack : PlayerAttacks)
	{
		if (!attack) { continue; }

		if (attack->CanExecute()) 
		{
			attack->Execute();
		}

		attack->Update(DeltaTime);
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

