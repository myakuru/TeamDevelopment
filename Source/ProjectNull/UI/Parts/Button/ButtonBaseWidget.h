#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonBaseWidget.generated.h"

class UButton;
class USoundBase;

/** デリゲート宣言(親デリゲート) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoveredButton);

/**
 * Widgetに使用するボタンのベース
 */
UCLASS()
class PROJECTNULL_API UButtonBaseWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UButton> Button;

	/** ホバー */
	UFUNCTION()
	void OnHoveredButton();

	virtual void DoHoveredButton();
	
	/** クリック */
	UFUNCTION()
	void OnClickedButton();

	virtual void DoClickedButton();
	
	/** 効果音 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Parameter|Sound|SE")
    TObjectPtr<USoundBase> HoverSound;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Parameter|Sound|SE")
	TObjectPtr<USoundBase> ClickSound;
	
public:

	//デリゲートの宣言
	UPROPERTY(BlueprintAssignable)
	FOnClickedButton OnClicked;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredButton OnHovered;
};
