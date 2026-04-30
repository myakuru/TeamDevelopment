// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHpBarWidget.generated.h"

class UImage;

UCLASS()
class PROJECTNULL_API UPlayerHpBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> HpImage;

public:
	/** プレイヤーのHPバーを更新する関数 */
	void SetHp(int32 CurrentHp, int32 MaxHp);
	
};
