
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerHUDWidget.generated.h"

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

	// 画像ウィジェットへのポインタ
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	class UImage* PlayerImage;

	void NativeConstruct() override;

	UFUNCTION()
	void OnClickedActionButton();

};
