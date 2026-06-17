#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ButtonBaseWidget.generated.h"

class UButton;

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
};
