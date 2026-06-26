// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponDetailWidget.h"
#include <ProjectNull/Weapon/Data/WeaponData.h>
#include <ProjectNull/Weapon/Instance/WeaponInstance.h>
#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Components/UniformGridPanel.h"
#include <ProjectNull/UI/OutGame/WeaponMenu/WeaponListItemWidget/WeaponListItemWidget.h>
#include <ProjectNull/UI/OutGame/WeaponMenu/EffectListItemWidget/EffectListItem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>

bool UWeaponDetailWidget::Initialize()
{
	Super::Initialize();

	WeaponName->SetText(FText());
	Icon->SetRenderOpacity(0.0f);

	EffectListItems.Empty();
	EffectList->ClearChildren();

	if (!EffectListItemClass)return false;

	for (int i = 0; i < 6; i++) {
		UEffectListItem* newItem = CreateWidget<UEffectListItem>(GetWorld(), EffectListItemClass);
		//newItem->Initialize();
		int32 row = i % 3;
		int32 column = i / 3;
		EffectList->AddChildToUniformGrid(newItem, row, column);
		EffectListItems.Add(newItem);
	}

    return false;
}

void UWeaponDetailWidget::SetWeaponData(const FWeaponData& InWeaponData)
{
	WeaponName->SetText(InWeaponData.DisplayName);
	Icon->SetBrushFromTexture(InWeaponData.Icon);
	Icon->SetRenderOpacity(1.0f);

	EffectListItems.Empty();
	EffectList->ClearChildren();

	if (!EffectListItemClass)return;

	for (int i = 0; i < 6; i++) {
		UEffectListItem* newItem = CreateWidget<UEffectListItem>(GetWorld(), EffectListItemClass);
		newItem->OnEffectListItemClicked.AddDynamic(this, &UWeaponDetailWidget::ListEffectOnClicked);
		//newItem->Initialize();
		int32 row = i % 3;
		int32 column = i / 3;
		EffectList->AddChildToUniformGrid(newItem, row, column);
		EffectListItems.Add(newItem);
	}

}

void UWeaponDetailWidget::SetWeaponInstance(const FWeaponInstance& InWeaponInstance)
{
	USuperGameInstance* gameInstance = GetGameInstance<USuperGameInstance>();
	if (!gameInstance)return;

	UWeaponManager* weaponManager = gameInstance->GetWeaponManager();
	if (!weaponManager)return;

	FWeaponData weaponData;
	if (!weaponManager->GetWeaponMaster(InWeaponInstance.WeaponId, weaponData))return;

	SetWeaponData(weaponData);

	for (int i = 0; i < InWeaponInstance.Effects.Num(); i++) {
		EffectListItems[i]->SetEffectInstance(InWeaponInstance.Effects[i]);
	}
}

void UWeaponDetailWidget::ListEffectOnClicked(const FEffectInstance& InEffectInstance)
{
	OnEffectListItemSelected.Broadcast(InEffectInstance);
}
