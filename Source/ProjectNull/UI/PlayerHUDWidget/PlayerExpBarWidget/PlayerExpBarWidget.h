// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerExpBarWidget.generated.h"

class UImage;
class ARobotController;

/**
 * プレイヤーの経験値バー変更
 */
UCLASS()
class PROJECTNULL_API UPlayerExpBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	/** 経験値を設定する関数 */
	void SetExp(float CurrentExp, float NextLevelExp);

	/** 経験値バーの虹色演出の表示・非表示を切り替える */
	void SetRainbowVisible(bool bVisible);

protected:

	virtual void NativeConstruct() override;


	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ExpImage;
	
	/** 経験値が最大になった際に虹色に光る用 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ExpRainbowImage;
	
	/** 虹色に光る用のMaterialインスタンス */
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> CooldownMaterial;
	
	/** 現在、強化画面がオープンしているかどうかのフラグのため */
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TObjectPtr<ARobotController> ExpUpgradeWidget;
	
private:
	float ExpRatio = 0.0f;

};
