
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

class UPlayerExpBarWidget;

UCLASS()
class PROJECTNULL_API UPlayerHUDWidget : public UUserWidget
{
	GENERATED_BODY()

protected:

	// HPテキストを表示するためのTextBlockウィジェットへのポインタ
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UTextBlock* HPText;

	// ボタンのウィジェットへのポインタ
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UButton* ActionButton;

	// 初期化処理を行うための関数
	virtual void NativeConstruct() override;

	UFUNCTION()
	void OnClickedActionButton();

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UPlayerExpBarWidget> PlayerExpBar;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<class UPlayerHpBarWidget> PlayerHpBar;

public:

	UFUNCTION(BlueprintCallable)
	void SetPlayerHp(int32 CurrentHp, int32 MaxHp);

	UFUNCTION(BlueprintCallable)
	void SetPlayerExp(int32 CurrentExp, int32 NextLevelExp);

};
