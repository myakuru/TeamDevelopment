#pragma once

#include "CoreMinimal.h"
#include "Engine/DeveloperSettings.h"
#include "UObject/SoftObjectPath.h"
#include "NullCommonMapsSettings.generated.h"

/**
 * ツールバーの「Common Maps」ドロップダウンに表示するマップ一覧。
 * プロジェクト設定 > Project > Common Editor Maps で編集できます。
 * (defaultconfig のため Config/DefaultEditor.ini に保存され、チームで共有されます)
 */
UCLASS(config=Editor, defaultconfig, meta=(DisplayName="Common Editor Maps"))
class UNullCommonMapsSettings : public UDeveloperSettings
{
	GENERATED_BODY()

public:
	//~UDeveloperSettings interface
	virtual FName GetCategoryName() const override { return FName("Project"); }

	/** ツールバーに表示するマップ一覧 */
	UPROPERTY(config, EditAnywhere, Category=Maps, meta=(AllowedClasses="/Script/Engine.World"))
	TArray<FSoftObjectPath> CommonEditorMaps;
};
