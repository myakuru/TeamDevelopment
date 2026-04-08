// Fill out your copyright notice in the Description page of Project Settings.


#include "GruntAnimInstance.h"
#include "ProjectNull/Actor/Character/Enemy/EnemyGrunt/EnemyGruntBase.h"

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