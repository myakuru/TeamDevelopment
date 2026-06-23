#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonBaseWidget.generated.h"

class UButton;

/** デリゲート宣言(親デリゲート) */
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnClickedButton);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHoveredButton);

/**
 * 
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

public:

	//デリゲートの宣言
	UPROPERTY(BlueprintAssignable)
	FOnClickedButton OnClicked;

	UPROPERTY(BlueprintAssignable)
	FOnHoveredButton OnHovered;
};
