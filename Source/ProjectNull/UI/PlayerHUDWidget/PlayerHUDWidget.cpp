#include "PlayerHUDWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

#include "Kismet/GameplayStatics.h"
#include "Components/Image.h"

// プレイヤーの経験値バーとHPバーのウィジェットクラスのヘッダーファイルをインクルード
#include <ProjectNull/UI/PlayerHUDWidget/PlayerExpBarWidget/PlayerExpBarWidget.h>

// プレイヤーのHPバーのウィジェットクラスのヘッダーファイルをインクルード
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHpBarWidget/PlayerHpBarWidget.h>

// タイマーのUIクラスのヘッダーファイルをインクルード
#include <ProjectNull/UI/PlayerHUDWidget/GameTimerWidget/GameTimerWidget.h>

void UPlayerHUDWidget::NativeConstruct()
{
	GameTimer->StartTimer(3600.0f);

	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnClickedActionButton);
	}
}

void UPlayerHUDWidget::OnClickedActionButton()
{
	const FString Message = TEXT("ActionButtonがクリックされました！");
	HPText->SetText(FText::FromString(Message));
}

void UPlayerHUDWidget::SetPlayerHp(int32 CurrentHp, int32 MaxHp)
{
	if (PlayerHpBar)
	{
		PlayerHpBar->SetHp(CurrentHp, MaxHp);
	}
}

void UPlayerHUDWidget::SetPlayerExp(int32 CurrentExp, int32 NextLevelExp)
{
	if (PlayerExpBar)
	{
		PlayerExpBar->SetExp(CurrentExp, NextLevelExp);
	}
}
