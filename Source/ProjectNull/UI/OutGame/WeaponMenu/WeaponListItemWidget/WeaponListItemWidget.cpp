// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponListItemWidget.h"
#include <Components/Border.h>
#include <Components/TextBlock.h>
#include <Components/Button.h>
#include <Components/Image.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>

void UWeaponListItemWidget::NativeConstruct()
{
	if (Button) {
		Button->OnClicked.AddUniqueDynamic(this, &UWeaponListItemWidget::OnButtonClicked);
		Button->OnHovered.AddUniqueDynamic(this, &UWeaponListItemWidget::OnButtonHovered);
		Button->OnUnhovered.AddUniqueDynamic(this, &UWeaponListItemWidget::OnButtonUnHovered);
	}
}

bool UWeaponListItemWidget::Initialize()
{
	Super::Initialize();

	if(WeaponName)WeaponName->SetText(FText());

	if(BackGround)BackGround->SetBrushColor(UnselectedBackGroundColor);
	if(Overlay)Overlay->SetBrushColor(UnHoveredOverlayColor);

	

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

	if (weaponManager->GetWeaponMaster(InWeaponInstance.WeaponId, WeaponData)) {
		SetWeaponData(WeaponData);
	}

}

void UWeaponListItemWidget::SetWeaponData(const FWeaponData& InWeaponData)
{
	WeaponData = InWeaponData;
	WeaponName->SetText(InWeaponData.DisplayName);
	Icon->SetBrushFromTexture(InWeaponData.Icon);
	
}

void UWeaponListItemWidget::OnButtonClicked()
{
	OnWeaponListItemClicked.Broadcast(WeaponInstance, WeaponData, this);
}

void UWeaponListItemWidget::OnButtonHovered()
{
	OnWeaponListItemHovered.Broadcast(WeaponInstance, WeaponData);
	Overlay->SetBrushColor(HoveredOverlayColor);
}

void UWeaponListItemWidget::OnButtonUnHovered()
{
	Overlay->SetBrushColor(UnHoveredOverlayColor);
}
