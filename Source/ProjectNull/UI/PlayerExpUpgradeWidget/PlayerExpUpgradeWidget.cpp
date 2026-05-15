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
	if (!bIsUpgradeWidgetUse) return;

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

FReply UPlayerExpUpgradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 左マウスボタンが押されたかチェック
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse Button Down!"));

		if (UpgradeWidget_0->IsMouseOver() || UpgradeWidget_1->IsMouseOver() || UpgradeWidget_2->IsMouseOver())
		{
			CloseWidget();
		}

		// イベントをこのウィジェットで処理したことを返す
		return FReply::Handled();
	}

	// 他のボタンの場合は親の処理に任せる
	return FReply::Unhandled();
}

void UPlayerExpUpgradeWidget::OpenWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);

	// 画面を一旦停止する
	UGameplayStatics::SetGamePaused(GetWorld(), true);

	// マウスカーソルを表示する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}
}

void UPlayerExpUpgradeWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	// 画面を再生する
	UGameplayStatics::SetGamePaused(GetWorld(), false);

	// マウスカーソルを非表示にする
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}
