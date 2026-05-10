// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpUpgradeWidget.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/UI/PlayerExpUpgradeWidget/ExpUpgradeWidget0/ExpUpgradeWidget0.h>

void UPlayerExpUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<USuperGameInstance>(GetGameInstance());
	if (GameInstance)
	{
		PlayerRuntimeData = GameInstance->GetPlayerRuntimeData();
	}

	OpenUpgradeWidget();
}

void UPlayerExpUpgradeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UPlayerExpUpgradeWidget::OpenUpgradeWidget()
{
	// 画面を一旦停止する
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// UI表示させる
	OpenWidget();

	// 強化画面１つ目表示
	if (ExpUpgradeWidget0)
	{
		ExpUpgradeWidget0->ChoicesExpUpgrade();
	}
}

void UPlayerExpUpgradeWidget::OpenWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
}

void UPlayerExpUpgradeWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);
}
