#include "PlayerHUDWidget.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>

// プレイヤーの経験値バーのUI
#include <ProjectNull/UI/PlayerHUDWidget/PlayerExpBarWidget/PlayerExpBarWidget.h>

// HpバーのUI
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHpBarWidget/PlayerHpBarWidget.h>

// ゲームのタイマー
#include <ProjectNull/UI/PlayerHUDWidget/GameTimerWidget/GameTimerWidget.h>

// プレイヤーのスキルのUI
#include <ProjectNull/UI/SkillWidgetBase/SkillWidgetBase.h>

void UPlayerHUDWidget::NativeConstruct()
{
	GameTimer->StartTimer(120.0f);

	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnClickedActionButton);
	}

	SetIsFocusable(false);

	SetPlayerSkillCooldown(SkillCooldownTime);
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	SkillCooldownTime -= InDeltaTime * 0.1f;

	SkillCooldownTime = std::clamp(SkillCooldownTime, 0.0f, 1.0f); 

	SetPlayerSkillCooldown(SkillCooldownTime);
}

void UPlayerHUDWidget::OnClickedActionButton()
{
	const FString Message = TEXT("ActionButtonI");
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

void UPlayerHUDWidget::SetPlayerSkillCooldown(float CooldownTime)
{
	if (PlayerSkill)
	{
		PlayerSkill->UpdateRotationImage(CooldownTime);
	}
}
