// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RewardPanel.generated.h"

/**
 * 
 */

class UVerticalBox;
class UButton;
class UWeaponListItemWidget;
class UWeaponDetailWidget;

struct FWeaponInstance;
struct FWeaponData;

/** リザルトページ進行デリゲート宣言マクロ */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FToStageSelectRequested);

UCLASS()
class PROJECTNULL_API URewardPanel : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct()override;
	bool Initialize()override;

	/** リザルトページ進行デリゲート */
	UPROPERTY(BlueprintAssignable)
	FToStageSelectRequested OnNextPageRequested;

private:

	/** ボタンクリックに登録する関数 */
	UFUNCTION()
	void OnToStageSelectButtonClicked();

	UFUNCTION()
	void OnWeaponListItemHovered(const FWeaponInstance& InWeaponInstance,const FWeaponData& InWeaponData);

	UPROPERTY(meta =(Bindwidget))
	TObjectPtr<UVerticalBox> RewardWeaponList;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> ToStageSelectButton;

	UPROPERTY(EditAnywhere)
	TSubclassOf<UWeaponListItemWidget> WeaponListItemWidgetClass;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UWeaponDetailWidget> WeaponDetail;

};
