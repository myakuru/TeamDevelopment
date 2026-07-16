#include "PlayerHUDWidget.h"
#include <Components/Button.h>

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
#include <ProjectNull/Stage/Manager/StageManager.h>

#include <ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>

// キャラクターパラメーターデータへの参照
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

// ギアチェンジのUI
#include <ProjectNull/UI/PlayerHUDWidget/GearChangeWidget/GearChangeWidget.h>

void UPlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	//タイマー開始（カウントダウンはStageManagerが所有）
	/*if (USuperGameInstance* GI = GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		if (UStageManager* StageManager = GI->GetStageManagerSubsystem())
		{
			StageManager->StartStageTimer();
		}
	}*/

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

void UPlayerHUDWidget::SetPlayerHp(float CurrentHp, float MaxHp)
{
	if (PlayerHpBar)
	{
		PlayerHpBar->SetHp(CurrentHp, MaxHp);
	}
}

void UPlayerHUDWidget::SetPlayerExp(float CurrentExp, float NextLevelExp)
{
	if (PlayerExpBar)
	{
		PlayerExpBar->SetExp(CurrentExp, NextLevelExp);
	}
}

void UPlayerHUDWidget::SetExpRainbowVisible(bool bVisible)
{
	if (PlayerExpBar)
	{
		PlayerExpBar->SetRainbowVisible(bVisible);
	}
}

void UPlayerHUDWidget::SetPlayerSkillCooldown(int32 SkillIndex, float CooldownTime, float MaxCooldown)
{
	if (SkillWidgets.IsValidIndex(SkillIndex) && SkillWidgets[SkillIndex])
	{
		// クールダウンのImageの回転
		SkillWidgets[SkillIndex]->UpdateRotationImage(CooldownTime, MaxCooldown);

		// クールダウン時間のテキスト
		SkillWidgets[SkillIndex]->UpdateCooldownText(CooldownTime);
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

	if (GameInstance && GameInstance->GetPlayerParameterData())
	{
		UPlayerParameterData* CharacterParameterData = GameInstance->GetPlayerParameterData();

		UPlayerRuntimeData* PlayerRuntimeDataInstance = GameInstance->GetPlayerRuntimeData();

		// HPのデリゲートを登録
		PlayerRuntimeDataInstance->OnHealthChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerHp);

		// 経験値のデリゲートを登録
		PlayerRuntimeDataInstance->OnExperienceChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerExp);

		// ギアエネルギーのデリゲートを登録
		PlayerRuntimeDataInstance->OnGearEnergyChanged.AddDynamic(this, &UPlayerHUDWidget::SetGearChangeEnergy);

		// スキルのクールダウンのデリゲートを登録
		CharacterParameterData->OnSkillCooldownChanged.AddDynamic(this, &UPlayerHUDWidget::SetPlayerSkillCooldown);
	}
}