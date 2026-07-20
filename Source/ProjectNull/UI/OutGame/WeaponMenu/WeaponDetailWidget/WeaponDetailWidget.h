// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <ProjectNull/Weapon/Instance/EffectInstance.h>
#include "WeaponDetailWidget.generated.h"

class UTextBlock;
class UImage;
class UBorder;
class UUniformGridPanel;
class UScrollBox;
class UEffectListItem;
struct FWeaponData;
struct FWeaponInstance;
struct FEffectInstance;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEffectListItemSelected, const FEffectInstance&, InEffectInstance);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class PROJECTNULL_API UWeaponDetailWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable)
	bool Initialize()override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponData(const FWeaponData& InWeaponData);

	UFUNCTION(BlueprintCallable)
	void SetWeaponInstance(const FWeaponInstance& InWeaponInstance);

	UPROPERTY(BlueprintAssignable)
	FOnEffectListItemSelected OnEffectListItemSelected;

private:

	UFUNCTION()
	void ListEffectOnClicked(const FEffectInstance& InEffectInstance);

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> WeaponName;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UScrollBox> EffectList;
	
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY()
	TArray<TObjectPtr<UEffectListItem>> EffectListItems;

protected:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UEffectListItem> EffectListItemClass;

};
