// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerExpUpgradeWidget.generated.h"

class USuperGameInstance;
class UPlayerRuntimeData;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UPlayerExpUpgradeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	/** ゲームインスタンスへの参照 */
	TObjectPtr<USuperGameInstance> GameInstance;

	/** プレイヤーのランタイムデータへの参照 */
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;

	virtual void NativeConstruct() override;

	/**
	 * @brief 強化が確定されたときに呼ばれる（サブクラスでオーバーライド）
	 */
	UFUNCTION(BlueprintNativeEvent, Category = "Upgrade")
	void OnUpgradeConfirmed() {};
	virtual void OnUpgradeConfirmed_Implementation() {};

public:

	/**
	 * @brief 強化画面を開く
	 */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void OpenWidget();

	/**
	 * @brief 強化画面を閉じる
	 */
	UFUNCTION(BlueprintCallable, Category = "Upgrade")
	virtual void CloseWidget();
	
};
