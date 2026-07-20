// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include <ProjectNull/Weapon/Data/WeaponData.h>
#include <ProjectNull/Weapon/Instance/WeaponInstance.h>
#include "WeaponListItemWidget.generated.h"

class UTextBlock;
class UButton;
class UBorder;
class UImage;
class UWeaponListItemWidget;

struct FWeaponInstance;
struct FWeaponData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnWeaponListItemClicked,const FWeaponInstance&,InWeaponInstance,const FWeaponData&, InWeaponData, UWeaponListItemWidget*,ClickedItem);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnWeaponListItemHovered,const FWeaponInstance&,InWeaponInstance,const FWeaponData&, InWeaponData);

/**
 * 
 */
UCLASS(Blueprintable,BlueprintType)
class PROJECTNULL_API UWeaponListItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	void NativeConstruct()override;
	
	UFUNCTION(BlueprintCallable)
	bool Initialize()override;

	UFUNCTION(BlueprintCallable)
	void SetSelectedState(bool IsSelected);

	UFUNCTION(BlueprintCallable)
	void SetWeaponInstance(const FWeaponInstance& InWeaponInstance);

	UFUNCTION(BlueprintCallable)
	void SetWeaponData(const FWeaponData& InWeaponData);

	UPROPERTY(BlueprintAssignable)
	FOnWeaponListItemClicked OnWeaponListItemClicked;

	UPROPERTY(BlueprintAssignable)
	FOnWeaponListItemHovered OnWeaponListItemHovered;

private:

	UFUNCTION()
	void OnButtonClicked();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnHovered();

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UTextBlock> WeaponName;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UButton> Button;

	UPROPERTY(meta =(BindWidget))
	TObjectPtr<UBorder> BackGround;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UBorder> Overlay;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UImage> Icon;

	UPROPERTY(EditAnywhere)
	FLinearColor HoveredOverlayColor;

	UPROPERTY(EditAnywhere)
	FLinearColor UnHoveredOverlayColor;

	UPROPERTY(EditAnywhere)
	FLinearColor SelectedBackGroundColor;

	UPROPERTY(EditAnywhere)
	FLinearColor UnselectedBackGroundColor;

	FWeaponInstance WeaponInstance;
	FWeaponData WeaponData;

};
