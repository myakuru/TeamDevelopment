// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GearChangeWidget.generated.h"

class UImage;
class UTextBlock;
class USuperGameInstance;

/**
 * ギアチェンジのUIを表示するウィジェットクラス
 */
UCLASS()
class PROJECTNULL_API UGearChangeWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	/** ギアチェンジのUIを表示するためのマテリアルインスタンス */
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> GearChangeMaterial;

	/** ギアチェンジのUIを表示するための画像 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> GearChangeImage;

	/** ギアチェンジのUIを表示するためのテキスト */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> GearChangeText;

	virtual void NativeConstruct() override;

public:

	UFUNCTION(BlueprintCallable)
	void SetGearChangeEnergy(float Charge);

private:
	/** ギアチェンジの最大チャージ量 */
	static constexpr float MaxChargeAmount = 1000.0f;

	/** ギアチェンジの最小のチャージ量 */
	static constexpr float MinChargeAmount = 0.0f;

};
