
#include "CombatCharacterBase.h"

ACombatCharacterBase::ACombatCharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ACombatCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACombatCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACombatCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ACombatCharacterBase::ApplyDamage(float Damage)
{
	CombatStats.HP.Current -= Damage;
}

