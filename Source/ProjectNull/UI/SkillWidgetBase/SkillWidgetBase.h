// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidgetBase.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API USkillWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SkillImage;

	// �N�[���_�E���p�̃}�e���A��
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CooldownMaterial;

	virtual void NativeConstruct() override;

public:

	// 0����1�͈̔͂ŃX�L���A�C�R�����]������֐�
	UFUNCTION(BlueprintCallable)
	void UpdateRotationImage(float angle);
};
