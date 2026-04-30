
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UPlayerExpBarWidget;
class UTextBlock;
class UButton;
class USuperGameInstance;
class UGearChangeWidget;
class USkillWidgetBase;
class UGameTimerWidget;
class UPlayerHpBarWidget;

UCLASS()
class PROJECTNULL_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UTextBlock> HPText;

	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	TObjectPtr<UButton> ActionButton;

	virtual void NativeConstruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UFUNCTION()
	void OnClickedActionButton();

	/** プレイヤーの経験値バー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerExpBarWidget> PlayerExpBar;

	/** プレイヤーのHPバー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerHpBarWidget> PlayerHpBar;

	/** ゲームのタイマー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGameTimerWidget> GameTimer;

	/** スキルWidget（インデックス順） */
	UPROPERTY(meta = (BindWidget))
	TArray<TObjectPtr<USkillWidgetBase>> SkillWidgets;

	/** スキルWidget0 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillWidgetBase> SkillWidget_0;

	/** スキルWidget1 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillWidgetBase> SkillWidget_1;

	/** スキルWidget2 */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<USkillWidgetBase> SkillWidget_2;

	/** ギアチェンジのUI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UGearChangeWidget> GearChange;

	/** ゲームインスタンスへの参照 */
	TObjectPtr<USuperGameInstance> GameInstance;

public:

	/**
	 * @brief プレイヤーのHPを更新する関数
	 * @param CurrentHp 現在のHP
	 * @param MaxHp 最大HP
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlayerHp(float CurrentHp, float MaxHp);

	/**
	 * @brief プレイヤーの経験値を更新する関数
	 * @param CurrentExp 現在の経験値
	 * @param NextLevelExp 次のレベルに必要な経験値
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlayerExp(int32 CurrentExp, int32 NextLevelExp);

	/**
	 * @brief プレイヤーのスキルのクールダウンを更新する関数
	 * @param CooldownTime クールダウン時間（0~1の範囲）
	 */
	UFUNCTION(BlueprintCallable)
	void SetPlayerSkillCooldown(int32 SkillIndex, float CooldownTime,float CooldownRunTime);

	/**
	 * @brief ギアチェンジエネルギーを設定します。
	 * @param Charge 設定するエネルギーの値
	 */
	UFUNCTION(BlueprintCallable)
	void SetGearChangeEnergy(float Charge);

private:
	/**
	 * @brief デリゲートを登録します
	 */
	void RegisterDelegates();

};
