// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerExpBarWidget.generated.h"

class UImage;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UPlayerExpBarWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ExpImage;

public:
	/** 経験値を設定する関数 */
	void SetExp(int32 CurrentExp, int32 NextLevelExp);
	
};
