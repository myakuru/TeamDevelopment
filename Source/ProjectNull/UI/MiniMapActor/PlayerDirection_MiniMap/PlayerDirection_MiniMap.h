// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerDirection_MiniMap.generated.h"

class UImage;

/**
 * プレイヤーの向きを表示するためのウィジェットクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerDirection_MiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:

	/** プレイヤーの向きを表示するための画像 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UImage> PlayerDirection;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:

	/** プレイヤーのUIの進行方向を変更 */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerDirection(float PlayerYaw);
	
};
