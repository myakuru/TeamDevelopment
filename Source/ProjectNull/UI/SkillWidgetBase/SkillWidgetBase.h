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

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CooldownMaterial;

	virtual void NativeConstruct() override;

public:

	/** スキルのクールダウンを更新 */
	UFUNCTION(BlueprintCallable)
	void UpdateRotationImage(float angle);
};
