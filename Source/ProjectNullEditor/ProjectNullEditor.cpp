// ProjectNull エディタ専用モジュール
// Lyra の LyraEditor モジュールと同様に、レベルエディタのツールバー（再生ボタンの右）に
// 「Common Maps」ドロップダウンを追加する。

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"
#include "ToolMenus.h"
#include "Editor.h"
#include "Subsystems/AssetEditorSubsystem.h"
#include "Framework/MultiBox/MultiBoxBuilder.h"
#include "Styling/AppStyle.h"
#include "NullCommonMapsSettings.h"

#define LOCTEXT_NAMESPACE "ProjectNullEditor"

// PIE中でなければ true
static bool HasNoPlayWorld()
{
	return GEditor->PlayWorld == nullptr;
}

static void OpenCommonMap_Clicked(const FString MapPath)
{
	if (ensure(MapPath.Len()))
	{
		GEditor->GetEditorSubsystem<UAssetEditorSubsystem>()->OpenEditorForAsset(MapPath);
	}
}

static TSharedRef<SWidget> GetCommonMapsDropdown()
{
	FMenuBuilder MenuBuilder(true, nullptr);

	for (const FSoftObjectPath& Path : GetDefault<UNullCommonMapsSettings>()->CommonEditorMaps)
	{
		if (!Path.IsValid())
		{
			continue;
		}

		MenuBuilder.AddMenuEntry(
			FText::FromString(Path.GetAssetName()),
			LOCTEXT("CommonPathDescription", "このマップをエディタで開く"),
			FSlateIcon(),
			FUIAction(
				FExecuteAction::CreateStatic(&OpenCommonMap_Clicked, Path.ToString()),
				FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
				FIsActionChecked(),
				FIsActionButtonVisible::CreateStatic(&HasNoPlayWorld)));
	}

	return MenuBuilder.MakeWidget();
}

static bool CanShowCommonMaps()
{
	return HasNoPlayWorld() && !GetDefault<UNullCommonMapsSettings>()->CommonEditorMaps.IsEmpty();
}

static void RegisterGameEditorMenus()
{
	UToolMenu* Menu = UToolMenus::Get()->ExtendMenu("LevelEditor.LevelEditorToolBar.PlayToolBar");
	FToolMenuSection& Section = Menu->AddSection("PlayGameExtensions",
		TAttribute<FText>(), FToolMenuInsert("Play", EToolMenuInsertType::After));

	FToolMenuEntry CommonMapEntry = FToolMenuEntry::InitComboButton(
		"CommonMapOptions",
		FUIAction(
			FExecuteAction(),
			FCanExecuteAction::CreateStatic(&HasNoPlayWorld),
			FIsActionChecked(),
			FIsActionButtonVisible::CreateStatic(&CanShowCommonMaps)),
		FOnGetContent::CreateStatic(&GetCommonMapsDropdown),
		LOCTEXT("CommonMaps_Label", "Common Maps"),
		LOCTEXT("CommonMaps_ToolTip", "よく使うマップを開く"),
		FSlateIcon(FAppStyle::GetAppStyleSetName(), "Icons.Level"));
	CommonMapEntry.StyleNameOverride = "CalloutToolbar";
	Section.AddEntry(CommonMapEntry);
}

class FProjectNullEditorModule : public FDefaultModuleImpl
{
	virtual void StartupModule() override
	{
		if (!IsRunningGame())
		{
			// ToolMenus の初期化完了後に登録される（初期化済みなら即時実行）
			UToolMenus::RegisterStartupCallback(
				FSimpleMulticastDelegate::FDelegate::CreateStatic(&RegisterGameEditorMenus));
		}
	}
};

IMPLEMENT_MODULE(FProjectNullEditorModule, ProjectNullEditor);

#undef LOCTEXT_NAMESPACE
