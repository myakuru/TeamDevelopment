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

	// ABPオーナーを取得
	AEnemyGruntBase* OwnerCharacter 
		= Cast<AEnemyGruntBase>(GetOwningActor());
	if (!OwnerCharacter) { return; }
	
	// OwnerChacterにまつわる処理
	// 攻撃可能フラグを取得する
	CanAttack = OwnerCharacter->CanAttack();
}