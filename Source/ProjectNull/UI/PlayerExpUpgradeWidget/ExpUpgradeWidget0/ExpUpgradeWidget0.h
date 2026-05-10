// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpUpgradeWidget0.generated.h"

class UTextBlock;

/**
 * データーテーブルから取得したテキストをUIにするクラス
 */
UCLASS()
class PROJECTNULL_API UExpUpgradeWidget0 : public UUserWidget
{
	GENERATED_BODY()

public:
	void ChoicesExpUpgrade();

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

private:
	int32 GetRandomTextId();
};
