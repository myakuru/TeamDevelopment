#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "OutGameBaseLevelScriptActor.generated.h"

/**
 * アウトゲームのベースレベルスクリプトアクター
 */
UCLASS()
class PROJECTNULL_API AOutGameBaseLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
protected:

	UFUNCTION(BlueprintCallable)
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "Widget")
	virtual void CreateOutGameWidget();

	/** ウィジェットのクラス */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widget", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class UUserWidget> CreateOutGameWidgetClass;
};
