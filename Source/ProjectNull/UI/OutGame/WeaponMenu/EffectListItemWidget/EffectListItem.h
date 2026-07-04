// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h" 
#include <ProjectNull/Weapon/Instance/EffectInstance.h>
#include "EffectListItem.generated.h"

class UButton;
class UTextBlock;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectListItemClicked,const FEffectInstance&,InEffectInstance);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class PROJECTNULL_API UEffectListItem : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	bool Initialize()override;

	UFUNCTION(BlueprintCallable)
	void SetEffectInstance(FEffectInstance InEffectInstance);

	UFUNCTION(BlueprintPure)
	const FEffectInstance& GetEffectInstance() { return EffectInstance; }

	UPROPERTY(BlueprintAssignable)
	FOnEffectListItemClicked OnEffectListItemClicked;

private:

	UPROPERTY(meta = (Bindwidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> EffectLevel;

	UFUNCTION()
	void OnClickedButton();

	FEffectInstance EffectInstance;
	
	
};
