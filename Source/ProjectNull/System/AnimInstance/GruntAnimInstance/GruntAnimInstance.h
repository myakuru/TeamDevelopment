// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GruntAnimInstance.generated.h"

/**
 * グラント(雑魚敵)のアニメーションインスタンス
 */
UCLASS()
class PROJECTNULL_API UGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	// 攻撃可能フラグ
	UPROPERTY(BlueprintReadWrite)
	bool CanAttack = false;

public:

	// 毎フレーム処理
	virtual void NativeUpdateAnimation(float DeltaTime)	override;
};
