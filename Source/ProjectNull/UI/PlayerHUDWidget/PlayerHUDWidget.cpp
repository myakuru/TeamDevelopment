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
#include <ProjectNull/UI/PlayerHUDWidget/SkillWidgetBase/SkillWidgetBase.h>

// ゲームインスタンスへの参照
#include <ProjectNull/GameInstance/SuperGameInstance.h>

// キャラクターパラメーターデータへの参照
#include <ProjectNull/Data/CharacterParameterData/CharacterParameterData.h>

// ギアチェンジのUI
#include <ProjectNull/UI/PlayerHUDWidget/GearChangeWidget/GearChangeWidget.h>

void UPlayerHUDWidget::NativeConstruct()
{
	GameTimer->StartTimer(120.0f);

	if (ActionButton)
	{
		ActionButton->OnClicked.AddDynamic(this, &UPlayerHUDWidget::OnClickedActionButton);
	}

	Super::NativeConstruct();

	// スキルは3つある想定で、配列にまとめる(今後増やすとき、ここに追加)
	SkillWidgets = { SkillWidget_0,SkillWidget_1,SkillWidget_2 };

	// スキルウィジェットにインデックスを設定
	for (int32 i = 0; i < SkillWidgets.Num(); ++i)
	{
		if (SkillWidgets[i])
		{
			SkillWidgets[i]->SkillIndex = i;
		}
	}

	// デリゲートの登録
	RegisterDelegates();

	SetIsFocusable(false);
}

void UPlayerHUDWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// デバック用
	if (GameInstance && GameInstance->GetCharacterParameterData())
	{
		UCharacterParameterData* CharacterParameterData = GameInstance->GetCharacterParameterData();

		for(int32 i = 0; i < SkillWidgets.Num(); ++i)
		{
			CharacterParameterData->UpdateSkillCooldown(i, InDeltaTime);
		}
	}
}

void UPlayerHUDWidget::OnClickedActionButton()
{
	const FString Message = TEXT("ActionButtonI");
	HPText->SetText(FText::FromString(Message));
}

void UPlayerHUDWidget::SetPlayerHp(float CurrentHp, float MaxHp)
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

void UPlayerHUDWidget::SetPlayerSkillCooldown(int32 SkillIndex, float CooldownTime, float CooldownRunTime)
{
	if (SkillWidgets.IsValidIndex(SkillIndex) && SkillWidgets[SkillIndex])
	{
		// クールダウンのImageの回転
		SkillWidgets[SkillIndex]->UpdateRotationImage(CooldownTime);

		// クールダウン時間のテキスト
		SkillWidgets[SkillIndex]->UpdateCooldownText(CooldownRunTime);
	}
}

void UPlayerHUDWidget::SetGearChangeEnergy(float Charge)
{
	if (GearChange)
	{
		GearChange->SetGearChangeEnergy(Charge);
	}
}

void UPlayerHUDWidget::RegisterDelegates()
{
	// ワールドからインスタンス所得
	GameInstance = Cast<USuperGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance && GameInstance->GetCharacterParameterData())
	{
		UCharacterParameterData* CharacterParameterData = GameInstance->GetCharacterParameterData();

		// HPのデリゲートを登録
		CharacterParameterData->OnHealthChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerHp);

		// 経験値のデリゲートを登録
		CharacterParameterData->OnExperienceChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerExp);

		// ギアエネルギーのデリゲートを登録
		CharacterParameterData->OnGearEnergyChanged.AddDynamic(this, &UPlayerHUDWidget::SetGearChangeEnergy);

		// スキルのクールダウンのデリゲートを登録
		CharacterParameterData->OnSkillCooldownChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerSkillCooldown);
	}
}
