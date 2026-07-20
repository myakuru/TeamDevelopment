#include "PlayerExpUpgradeWidget.h"
#include "Components/AudioComponent.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>
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

namespace
{
	/** 強化画面に提示する選択肢の数 */
	constexpr int32 UpgradeChoiceCount = 3;
}

void UPlayerExpUpgradeWidget::NativeConstruct()
{
	Super::NativeConstruct();

	GameInstance = Cast<USuperGameInstance>(GetGameInstance());

	PlayerRuntimeData = GameInstance ? GameInstance->GetPlayerRuntimeData() : nullptr;

	SetVisibility(ESlateVisibility::Hidden);
	SetIsEnabled(false);

	bIsUpgradeWidgetOpen = false;
	
	UpgradeWidget_0->SetHoverSESound(HoverSESound);
	UpgradeWidget_1->SetHoverSESound(HoverSESound);
	UpgradeWidget_2->SetHoverSESound(HoverSESound);
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
	if (!PlayerRuntimeData) return;

	// 抽選・レベル判定はランタイムデータ側が担当。UIは受け取って表示するだけ
	const TArray<FValidUpgradeInfo> Choices = PlayerRuntimeData->BuildUpgradeChoices(UpgradeChoiceCount);

	TArray<UExpUpgradeWidgetBase*> Widgets = { UpgradeWidget_0, UpgradeWidget_1, UpgradeWidget_2 };

	// すべてのWidgetを初期状態では非表示にしておく
	for (UExpUpgradeWidgetBase* Widget : Widgets)
	{
		if (Widget) Widget->SetVisibility(ESlateVisibility::Hidden);
	}

	for (int32 i = 0; i < Choices.Num() && i < Widgets.Num(); ++i)
	{
		if (!Widgets[i]) continue;
		Widgets[i]->SetDescriptionText(Choices[i].Description);
		Widgets[i]->SetUpgradeRowName(Choices[i].RowName);
		Widgets[i]->SetVisibility(ESlateVisibility::Visible);
	}

	bIsUpgradeWidgetFilledArray = (Choices.Num() > 1);
}

void UPlayerExpUpgradeWidget::OpenUpgradeWidget()
{
	if (!bIsUpgradeWidgetUse) return;

	if (!bIsUpgradeWidgetFilledArray) return;

	// UI表示させる
	OpenWidget();

	ChoicesExpUpgrade();
	
	//BGM再生
	OpenWidgetSEAudioComponent = 
		GetWorld()->GetGameInstance<USuperGameInstance>()->
		GetSoundManager()->Spawn2D(OpenWidgetSESound);
	
	if (OpenWidgetSEAudioComponent)
	{
		OpenWidgetSEAudioComponent->bIsUISound = true;
	}
}

FReply UPlayerExpUpgradeWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// 左マウスボタンが押されたかチェック
	if (InMouseEvent.GetEffectingButton() == EKeys::LeftMouseButton)
	{
		//クリック効果音
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(
				ClickSESound,1.0f,1.0f,0.0f,
				nullptr,nullptr,true);
		
		UExpUpgradeWidgetBase* Widgets[] = { UpgradeWidget_0, UpgradeWidget_1, UpgradeWidget_2 };

		for (UExpUpgradeWidgetBase* Widget : Widgets)
		{
			if (Widget && Widget->IsMouseOver())
			{
				// 選択された強化を適用する。効果の反映はランタイムデータ側の責務
				if (PlayerRuntimeData)
				{
					PlayerRuntimeData->ApplySelectedUpgrade(Widget->GetUpgradeRowName());
				}
				break;
			}
		}

		//BGM停止
		if (OpenWidgetSEAudioComponent)
		{
			OpenWidgetSEAudioComponent->Stop();
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

void UPlayerExpUpgradeWidget::SetExpBarRainbow(bool bVisible)
{
	ARobotController* RobotController = Cast<ARobotController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (!RobotController) return;

	if (UPlayerHUDWidget* PlayerHUD = RobotController->GetPlayerHUD())
	{
		PlayerHUD->SetExpRainbowVisible(bVisible);
	}
}
