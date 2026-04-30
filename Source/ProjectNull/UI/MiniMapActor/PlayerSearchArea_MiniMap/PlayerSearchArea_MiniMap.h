// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSearchArea_MiniMap.generated.h"

/**
 * プレイヤーの索敵範囲を表示するためのウィジェットクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerSearchArea_MiniMap : public UUserWidget
{
	GENERATED_BODY()

protected:
	/** プレイヤーの索敵範囲を表示するための画像 */
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* PlayerSearchArea;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	/** プレイヤーのUIの索敵範囲を変更 */
	UFUNCTION(BlueprintCallable)
	void UpdatePlayerSearchArea(float CameraYaw);
	
};
