// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAnimInstance.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyGrunt/EnemyGruntBase.h>

UGruntAnimInstance::UGruntAnimInstance():
	CanAttack(false)
{
}

void UGruntAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	// ABP�I�[�i�[��擾
	AEnemyGruntBase* OwnerCharacter 
		= Cast<AEnemyGruntBase>(GetOwningActor());
	if (!OwnerCharacter) { return; }
	
	// OwnerChacter�ɂ܂�鏈��
	// �U���\�t���O��擾����
	CanAttack = OwnerCharacter->CanAttack();
}