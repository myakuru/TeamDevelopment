// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerExpUpgradeWidget.h"
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/UI/PlayerExpUpgradeWidget/ExpUpgradeWidgetBase/ExpUpgradeWidgetBase.h>
#include "Components/Image.h"
#include "Components/CanvasPanel.h"
#include <ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h>
#include "Input/Events.h"
#include "Input/Reply.h"
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>

void UPlayerExpUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<USuperGameInstance>(GetGameInstance());

	PlayerRuntimeData = GameInstance ? GameInstance->GetPlayerRuntimeData() : nullptr;

	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	bIsUpgradeWidgetOpen = false;
}

UDataTable* UPlayerExpUpgradeWidget::GetExpUpgradeTable()
{
	if (CachedExpUpgradeTable) return CachedExpUpgradeTable;

	CachedExpUpgradeTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));
	if (!CachedExpUpgradeTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DT_ExpUpgrade がロードできません"));
	}
	return CachedExpUpgradeTable;
}

void UPlayerExpUpgradeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UpgradeWidget_0)
	{
		UpgradeWidget_0->UpdateScale();
	}
	if (UpgradeWidget_1)
	{
		UpgradeWidget_1->UpdateScale();
	}
	if (UpgradeWidget_2)
	{
		UpgradeWidget_2->UpdateScale();
	}

	// 背景の黒い画像のフェードイン処理
	BackgroundImageFadeIn();
}

void UPlayerExpUpgradeWidget::ChoicesExpUpgrade()
{
	UDataTable* Table = GetExpUpgradeTable();
	if (!Table) return;

	// キャッシュされたテーブルをロード（既にあるならそのまま）
	if (!CachedExpUpgradeTable)
	{
		CachedExpUpgradeTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));
		if (!CachedExpUpgradeTable)
		{
			UE_LOG(LogTemp, Warning, TEXT("DT_ExpUpgrade がロードできません"));
			return;
		}
	}

	// 行名を取得してランダムに3つ選択
	TArray<FName> AllRowNames = CachedExpUpgradeTable->GetRowNames();
	if (AllRowNames.Num() < 3) return;

	TArray<FName> SelectedRowNames;

	for (int32 i = 0; i < 3; ++i)
	{
		int32 index = FMath::RandRange(0, AllRowNames.Num() - 1);
		SelectedRowNames.Add(AllRowNames[index]);
		AllRowNames.RemoveAt(index);
	}

	ValidUpgrades.Empty(); // 既存の情報をクリア

	for (const FName& RowName : SelectedRowNames)
	{
		const FExpUpgradeRow* RowData = CachedExpUpgradeTable->FindRow<FExpUpgradeRow>(RowName, TEXT(""));
		if (!RowData) continue;

		// 現在のレベルに対応する説明文があるかチェック
		FName CurrentLevel = PlayerRuntimeData->GetUpgradeLevel(RowName);
		const int32 LevelIndex = FCString::Atoi(*CurrentLevel.ToString());

		if (RowData->UpgradeLevels.IsValidIndex(LevelIndex))
		{
			FValidUpgradeInfo Info;

			Info.RowName = RowName;
			Info.Multiplier = RowData->UpgradeLevels[LevelIndex].AttackMultiplier;
			Info.Description = RowData->UpgradeLevels[LevelIndex].Description;
			Info.CurrentLevel = CurrentLevel;

			ValidUpgrades.Add(Info);
		}
	};

	TArray<UExpUpgradeWidgetBase*> Widgets = { UpgradeWidget_0, UpgradeWidget_1, UpgradeWidget_2 };

	// すべてのWidgetを初期状態では非表示にしておく
	UpgradeWidget_0->SetVisibility(ESlateVisibility::Hidden);
	UpgradeWidget_1->SetVisibility(ESlateVisibility::Hidden);
	UpgradeWidget_2->SetVisibility(ESlateVisibility::Hidden);

	for (int32 i = 0; i < ValidUpgrades.Num() && i < Widgets.Num(); ++i)
	{
		Widgets[i]->SetDescriptionText(ValidUpgrades[i].Description);
		Widgets[i]->SetUpgradeRowName(ValidUpgrades[i].RowName);
		Widgets[i]->SetValidUpgradesMultiplier(ValidUpgrades[i].Multiplier);
		Widgets[i]->SetVisibility(ESlateVisibility::Visible);
	}

	if (ValidUpgrades.Num() <= 1)
	{
		bIsUpgradeWidgetFilledArray = false;
	}
	else
	{
		bIsUpgradeWidgetFilledArray = true;
	}

}

void UPlayerExpUpgradeWidget::OpenUpgradeWidget()
{
	if (!bIsUpgradeWidgetUse) return;

	if (!bIsUpgradeWidgetFilledArray) return;

	// UI表示させる
	OpenWidget();

	ChoicesExpUpgrade();
}

FReply UPlayerExpUpgradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 左マウスボタンが押されたかチェック
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		UE_LOG(LogTemp, Warning, TEXT("Mouse Button Down!"));

		if (UpgradeWidget_0->IsMouseOver())
		{
			PlayerRuntimeData->UpdateUpgradeStates(UpgradeWidget_0->GetUpgradeRowName());

			// ここで攻撃力の乗数をプレイヤーのランタイムデータに渡す
			PlayerRuntimeData->UpgradeAttackMultiplier(UpgradeWidget_0->GetUpgradeRowName(), UpgradeWidget_0->GetValidUpgradesMultiplier());
		}
		else if (UpgradeWidget_1->IsMouseOver())
		{
			PlayerRuntimeData->UpdateUpgradeStates(UpgradeWidget_1->GetUpgradeRowName());

			// ここで攻撃力の乗数をプレイヤーのランタイムデータに渡す
			PlayerRuntimeData->UpgradeAttackMultiplier(UpgradeWidget_1->GetUpgradeRowName(), UpgradeWidget_1->GetValidUpgradesMultiplier());
		}
		else if (UpgradeWidget_2->IsMouseOver())
		{
			PlayerRuntimeData->UpdateUpgradeStates(UpgradeWidget_2->GetUpgradeRowName());

			// ここで攻撃力の乗数をプレイヤーのランタイムデータに渡す
			PlayerRuntimeData->UpgradeAttackMultiplier(UpgradeWidget_2->GetUpgradeRowName(), UpgradeWidget_2->GetValidUpgradesMultiplier());
		}

		CloseWidget();

		// イベントをこのウィジェットで処理したことを返す
		return FReply::Handled();
	}

	// 他のボタンの場合は親の処理に任せる
	return FReply::Unhandled();
}

bool UPlayerExpUpgradeWidget::NowOpenWidget()
{
	if (bIsOpen) return true;
	
	return false;	
}

void UPlayerExpUpgradeWidget::OpenWidget()
{
	SetVisibility(ESlateVisibility::Visible);
	SetIsEnabled(true);
	
	// 現在ウィジットがオープンしている
	bIsOpen = true;

	// 強化画面が開いている間、経験値バーを虹色に光らせる
	SetExpBarRainbow(true);

	// 強化画面の初期化
	InitUpgradeWidget();

	// マウスカーソルを表示する
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = true;
		PlayerController->SetInputMode(FInputModeUIOnly());
	}

	// 画面を一旦停止する
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void UPlayerExpUpgradeWidget::CloseWidget()
{
	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	bIsOpen = false;

	// 強化画面を閉じたら虹色演出を消す
	SetExpBarRainbow(false);

	// マウスカーソルを非表示にする
	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (PlayerController)
	{
		PlayerController->bShowMouseCursor = false;
		PlayerController->FlushPressedKeys();
		PlayerController->SetIgnoreMoveInput(false);
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}

	// 画面を再生する
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UPlayerExpUpgradeWidget::BackgroundImageFadeIn()
{
	if (BackgroundImage)
	{
		// フェードインのアルファ値を更新
		BackgroundFadeInAlpha += GetWorld()->GetDeltaSeconds() * BackgroundFadeInDuration;
		BackgroundFadeInAlpha = FMath::Clamp(BackgroundFadeInAlpha, 0.0f, BackgroundFadeInAlphaMax);

		BackgroundImage->SetColorAndOpacity(FLinearColor(0.0f, 0.0f, 0.0f, BackgroundFadeInAlpha));
	}
}

void UPlayerExpUpgradeWidget::InitUpgradeWidget()
{
	if (BackgroundFadeInAlpha >= BackgroundFadeInAlphaMax)
	{
		BackgroundFadeInAlpha = 0.0f;
	}

	if(UpgradeWidget_0)
	{
		UpgradeWidget_0->InitExpUpgradeWidget();
	}

	if(UpgradeWidget_1)
	{
		UpgradeWidget_1->InitExpUpgradeWidget();
	}

	if(UpgradeWidget_2)
	{
		UpgradeWidget_2->InitExpUpgradeWidget();
	}

}

void UPlayerExpUpgradeWidget::SetAttackMultiplier(float Multiplier)
{

}

void UPlayerExpUpgradeWidget::SetExpBarRainbow(bool bVisible)
{
	ARobotController* RobotController = Cast<ARobotController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!RobotController) return;

	if (UPlayerHUDWidget* PlayerHUD = RobotController->GetPlayerHUD())
	{
		PlayerHUD->SetExpRainbowVisible(bVisible);
	}
}
