// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExpUpgradeWidgetBase.generated.h"

class UTextBlock;
class UExpUpgradeWidget0;

/**
 * データーテーブルから取得したテキストをUIにするクラスのベースクラス
 */
UCLASS()
class PROJECTNULL_API UExpUpgradeWidgetBase : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;
	virtual void ChoicesExpUpgrade();

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> UpgradeText;

private:
	virtual int32 GetRandomTextId();
};
