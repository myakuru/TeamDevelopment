// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpUpgradeWidget.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/UI/PlayerExpUpgradeWidget/ExpUpgradeWidgetBase/ExpUpgradeWidgetBase.h>

void UPlayerExpUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<USuperGameInstance>(GetGameInstance());

	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	if (GameInstance)
	{
		PlayerRuntimeData = GameInstance->GetPlayerRuntimeData();
	}
}

void UPlayerExpUpgradeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UpgradeWidget_0)
	{
		UpgradeWidget_0->ImageRotation();
	}
	if (UpgradeWidget_1)
	{
		UpgradeWidget_1->ImageRotation();
	}
	if (UpgradeWidget_2)
	{
		UpgradeWidget_2->ImageRotation();
	}
}

void UPlayerExpUpgradeWidget::OpenUpgradeWidget()
{
	// 画面を一旦停止する
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// マウスカーソルを表示する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}

	// UI表示させる
	OpenWidget();

	if (UpgradeWidget_0)
	{
		UpgradeWidget_0->ChoicesExpUpgrade();
	}

	if (UpgradeWidget_1)
	{
		UpgradeWidget_1->ChoicesExpUpgrade();
	}

	if (UpgradeWidget_2)
	{
		UpgradeWidget_2->ChoicesExpUpgrade();
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
