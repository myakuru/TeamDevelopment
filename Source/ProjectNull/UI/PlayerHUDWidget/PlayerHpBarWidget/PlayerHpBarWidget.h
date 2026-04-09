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

	// 画像ウィジェットへのポインタ
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> ExpImage;

public:

	void SetHp(int32 CurrentHp, int32 MaxHp);
	
};
