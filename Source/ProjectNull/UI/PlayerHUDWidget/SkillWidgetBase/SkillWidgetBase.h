// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SkillWidgetBase.generated.h"

class UImage;
class UTextBlock;

/**
 * プレイヤーのスキルのUIの基底クラス
 */
UCLASS()
class PROJECTNULL_API USkillWidgetBase : public UUserWidget
{
	GENERATED_BODY()

protected:
	/**
	 * @brief クールダウン表示する、アイコン
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> SkillImage;

	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CooldownMaterial;

	/**
	 * @brief クールダウン表示する、テキスト
	 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> CooldownText;

	virtual void NativeConstruct() override;

public:
	/** 表示位置を更新、PlayerHUDWidgetで変更されてる*/
	UFUNCTION(BlueprintCallable)
	void UpdateRotationImage(float CooldownTime);

	/** クールダウンテキストを更新する関数 */
	UFUNCTION(BlueprintCallable)
	void UpdateCooldownText(float CooldownTime);

	/** このWidgetが担当するスキルのインデックス（0〜2,PlayerHUDWidgetで変更されてる） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	int32 SkillIndex = 0;
};
