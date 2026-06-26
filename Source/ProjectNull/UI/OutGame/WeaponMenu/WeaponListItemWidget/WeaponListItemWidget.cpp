// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponListItemWidget.h"
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>

bool UWeaponListItemWidget::Initialize()
{
	Super::Initialize();

	WeaponName->SetText(FText());

	if (Button) {
		Button->OnClicked.AddDynamic(this, &UWeaponListItemWidget::OnButtonClicked);
	}

	return false;
}

void UWeaponListItemWidget::SetSelectedState(bool IsSelected)
{
	if (IsSelected) BackGround->SetBrushColor(SelectedBackGroundColor);
	else BackGround->SetBrushColor(UnselectedBackGroundColor);
}

void UWeaponListItemWidget::SetWeaponInstance(const FWeaponInstance& InWeaponInstance)
{
	WeaponInstance = InWeaponInstance;
	
	USuperGameInstance* gameInstance = GetGameInstance<USuperGameInstance>();
	if (!gameInstance)return;
	UWeaponManager* weaponManager = gameInstance->GetWeaponManager();
	if (!weaponManager)return;

	if (weaponManager->GetWeaponMaster(WeaponInstance.WeaponId, WeaponData)) {
		WeaponName->SetText(WeaponData.DisplayName);
	}

}

void UWeaponListItemWidget::SetWeaponData(const FWeaponData& InWeaponData)
{
	WeaponData = InWeaponData;
	WeaponName->SetText(InWeaponData.DisplayName);
}

void UWeaponListItemWidget::OnButtonClicked()
{
	OnWeaponListItemClicked.Broadcast(WeaponInstance, WeaponData, this);
}
