
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UPlayerExpBarWidget;
class UTextBlock;
class UButton;

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
	TObjectPtr<class UPlayerHpBarWidget> PlayerHpBar;

	/** ゲームのタイマー */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UGameTimerWidget> GameTimer;

	/** プレイヤーのスキルのUI */
	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class USkillWidgetBase> PlayerSkill;

	/** スキルのクールダウン時間（秒単位） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Skill")
	float SkillCooldownTime = 0.0f;


public:

	UFUNCTION(BlueprintCallable)
	void SetPlayerHp(int32 CurrentHp, int32 MaxHp);

	UFUNCTION(BlueprintCallable)
	void SetPlayerExp(int32 CurrentExp, int32 NextLevelExp);

	UFUNCTION(BlueprintCallable)
	void SetPlayerSkillCooldown(float CooldownTime);

};
