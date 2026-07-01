// Fill out your copyright notice in the Description page of Project Settings.


#include "RewardPanel.h"
#include <Components/Button.h>
#include <Components/VerticalBox.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Result/ResultManager/ResultManager.h>
#include <ProjectNull/Weapon/Instance/WeaponInstance.h>
#include <ProjectNull/UI/OutGame/WeaponMenu/WeaponListItemWidget/WeaponListItemWidget.h>
#include <ProjectNull/UI/OutGame/WeaponMenu/WeaponDetailWidget/WeaponDetailWidget.h>

void URewardPanel::NativeConstruct()
{
	Super::NativeConstruct();

	// 報酬画面移行ボタンのクリックイベントにページ進行関数をセット
	if (ToStageSelectButton) {
		ToStageSelectButton->OnClicked.AddUniqueDynamic(
			this,
			&URewardPanel::OnToStageSelectButtonClicked
		);
	}
}

bool URewardPanel::Initialize()
{
	Super::Initialize();

	RewardWeaponList->ClearChildren();
	WeaponDetail->SetVisibility(ESlateVisibility::Collapsed);

	USuperGameInstance* gameInstance = GetGameInstance<USuperGameInstance>();
	if (!gameInstance)return false;

	UResultManager* resultManager = gameInstance->GetResultManager();
	if (!resultManager)return false;

	if (!WeaponListItemWidgetClass)return false;
	for (const FWeaponInstance& weapon : resultManager->GetRewardWeapons()) {
		UWeaponListItemWidget* newItem = CreateWidget<UWeaponListItemWidget>(GetWorld(),WeaponListItemWidgetClass);

		newItem->OnWeaponListItemHovered.AddUniqueDynamic(this, &URewardPanel::OnWeaponListItemHovered);
		newItem->Initialize();
		newItem->SetWeaponInstance(weapon);
		RewardWeaponList->AddChild(newItem);
	}


	return false;
}

void URewardPanel::OnToStageSelectButtonClicked()
{
	OnNextPageRequested.Broadcast();
}

void URewardPanel::OnWeaponListItemHovered(const FWeaponInstance& InWeaponInstance, const FWeaponData& InWeaponData)
{
	if (!WeaponDetail)return; 

	WeaponDetail->SetVisibility(ESlateVisibility::Visible);
	WeaponDetail->SetWeaponInstance(InWeaponInstance);
}
