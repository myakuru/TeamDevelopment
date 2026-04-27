// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GruntAnimInstance.generated.h"

/**
 * �O�����g(�G���G)�̃A�j���[�V�����C���X�^���X
 */
UCLASS()
class PROJECTNULL_API UGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

protected:

	// �U���\�t���O
	UPROPERTY(BlueprintReadWrite)
	bool CanAttack = false;

public:

	UGruntAnimInstance();

	// ���t���[������
	virtual void NativeUpdateAnimation(float DeltaTime)	override;
};
