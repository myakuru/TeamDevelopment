#include "StageSelectHUDWidget.h"
#include "Components/CanvasPanel.h"
#include "Components/CanvasPanelSlot.h"
#include "Components/TextBlock.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

//ゲームインスタンス
#include <ProjectNull/GameInstance/SuperGameInstance.h>
//セーブデータ
#include <ProjectNull/SaveGame/MySaveGame.h>
//ステージのデータアセット
#include<ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
//ステージマネージャー
#include<ProjectNull/Stage/Manager/StageManager.h>
//ステージボタンUI
#include<ProjectNull/UI/OutGame/StageSelectHUDWidget/StageSelectHUDParts/StageButtonWidget.h>

void UStageSelectHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CurrentSelectedStageIndex = 0;

	//ステージデータをゲームインスタンスから取得しておく
	StageDataAsset = GetWorld()->GetGameInstance<USuperGameInstance>()
		->GetStageManagerSubsystem()->GetStageDataAsset();
	if (!StageDataAsset)
	{
		UE_LOG(LogTemp, Error, TEXT("StageDataAsset is null in StageSelectHUDWidget!"));
		return;
	}

	if (ReturnButton)
	{
		ReturnButton->OnClicked.AddDynamic(this, &UStageSelectHUDWidget::OnClickedReturnButton);
	}

	ChangeStageDetails(CurrentSelectedStageIndex);

	CreateStageButtons();
}

void UStageSelectHUDWidget::CreateStageButtons()
{
	if (!StageDataAsset)return;

	auto* SaveData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetCurrentSaveData();

	//解放済みステージの数だけステージボタンを生成していく
	for (int32 i = 0; i < StageDataAsset->StageData.Num(); i++)
	{
		if (!SaveData || !SaveData->StageProgressList.IsValidIndex(i)) break;

		// 解放フラグを取得（現在のステージが解放されているか）
		bool bIsUnlocked = SaveData->StageProgressList[i].bUnlocked;

		auto* ButtonWidget =
			CreateWidget<UStageButtonWidget>(this, StageButtonWidgetClass);

		if (!ButtonWidget)continue;

		ButtonWidget->Setup(i, bIsUnlocked);

		//デリゲートのバインド
		ButtonWidget->OnHovered.AddUniqueDynamic(
			this, &UStageSelectHUDWidget::OnHoveredStageButton);

		ButtonWidget->OnClicked.AddUniqueDynamic(
			this, &UStageSelectHUDWidget::OnClickedStageButton);

		UCanvasPanelSlot* CanvasSlot =
			StageCanvas->AddChildToCanvas(ButtonWidget);

		if (!CanvasSlot)continue;

		CanvasSlot->SetPosition(StageButtonWidgetFirstPosition + (StageButtonWidgetInterval * i));

		if (!bIsUnlocked)break;
	}
}

void UStageSelectHUDWidget::OnClickedStageButton(int32 InStageIndex)
{
	if (!StageDataAsset || StageDataAsset->StageData.Num() == 0)return;

	if (CurrentSelectedStageIndex != InStageIndex) {
		CurrentSelectedStageIndex = ClampStageIndex(InStageIndex);
	}

	//StageManagerSubsystem：ステージ開始
	GetWorld()->GetGameInstance<USuperGameInstance>()
		->GetStageManagerSubsystem()->StageStart(CurrentSelectedStageIndex);

	UGameplayStatics::OpenLevel(this, FName(StageDataAsset->StageData[CurrentSelectedStageIndex].LevelName));
}

void UStageSelectHUDWidget::OnHoveredStageButton(int32 InStageIndex)
{
	CurrentSelectedStageIndex = ClampStageIndex(InStageIndex);

	ChangeStageDetails(CurrentSelectedStageIndex);
}

void UStageSelectHUDWidget::OnClickedReturnButton()
{
	UGameplayStatics::OpenLevel(this, "MainHubLevel");
}

int32 UStageSelectHUDWidget::ClampStageIndex(int32 InStageIndex)
{
	int32 stageIndex = InStageIndex;

	stageIndex = FMath::Clamp(stageIndex, 0,
		StageDataAsset->StageData.Num() - 1);

	return stageIndex;
}

bool UStageSelectHUDWidget::CheckStageDetailsExistence()
{
	if (!StageNameText || !StageImage)return false;
	if (!StageDataAsset || StageDataAsset->StageData.Num() == 0)return false;

	return true;
}

bool UStageSelectHUDWidget::CheckStageData(int32 CheckStageIndex)
{
	if (!StageDataAsset || StageDataAsset->StageData.Num() == 0)return false;

	if (!StageDataAsset->StageData.IsValidIndex(CheckStageIndex))return false;

	return true;
}

void UStageSelectHUDWidget::ChangeStageDetails(int32 InStageIndex)
{
	if (!CheckStageDetailsExistence())return;

	//ステージインデックスをクランプ
	int32 stageIndex = ClampStageIndex(InStageIndex);

	//ステージのデーブデータをゲームインスタンスから取得しておく
	auto* SaveData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetCurrentSaveData();

	//ステージ詳細更新
	//ステージ名
	StageNameText->SetText(StageDataAsset->StageData[stageIndex].StageName);

	//ステージ画像
	ChangeStageImageTexture(StageDataAsset->StageData[stageIndex].StageImage);

	ChangeHighScoreText(stageIndex);

	//ステージの進捗
	ChangeProgressText(stageIndex);
}

void UStageSelectHUDWidget::ChangeStageImageTexture(UTexture2D* NewImage)
{
	if (!StageImage || !NewImage)return;

	StageImage->SetBrushFromTexture(NewImage);
}

void UStageSelectHUDWidget::ChangeHighScoreText(int32 InStageIndex)
{
	if (!HighScoreText)return;

	//ステージインデックスをクランプ
	int32 stageIndex = ClampStageIndex(InStageIndex);

	int32 HighScoreValue = 0;

	// セーブデータから現在のステージのハイスコアを取得する（仮の実装）
	auto* SaveData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetCurrentSaveData();
	if (SaveData && SaveData->StageProgressList.IsValidIndex(stageIndex))
	{
		HighScoreValue = SaveData->StageProgressList[stageIndex].HighScore;
	}

	FText FinalText = FText::Format(
		HighScoreFrontText, 
		FText::AsNumber(HighScoreValue));

	// UTextBlockにセット
	HighScoreText->SetText(FinalText);
}

void UStageSelectHUDWidget::ChangeProgressText(int32 InStageIndex)
{
	//ステージインデックスをクランプ
	int32 stageIndex = ClampStageIndex(InStageIndex);

	// 安全のため、テクスチャが設定されているかチェック
	if (!MissionClearedTexture || !MissionNotClearedTexture) return;

	// 表示用の状態配列を初期化（デフォルトはすべて未クリア）
	TArray<bool> CurrentStageMissions = { false, false, false };

	// セーブデータから進行度を取得
	auto* SaveData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetCurrentSaveData();
	if (SaveData && SaveData->StageProgressList.IsValidIndex(stageIndex))
	{
		// セーブデータ側に保存されているミッション配列を取得
		const TArray<bool>& SavedMissions = SaveData->StageProgressList[stageIndex].MissionClears;

		for (int32 i = 0; i < 3; i++)
		{
			if (SavedMissions.IsValidIndex(i))
			{
				CurrentStageMissions[i] = SavedMissions[i];
			}
		}
	}

	// 取得したフラグに応じて、それぞれのUImageのテクスチャを切り替える
	if (StageClearAchievementImage)
		StageClearAchievementImage->SetBrushFromTexture(CurrentStageMissions[0] ? MissionClearedTexture : MissionNotClearedTexture);

	if (MidBossAppearsAchievementImage)
		MidBossAppearsAchievementImage->SetBrushFromTexture(CurrentStageMissions[1] ? MissionClearedTexture : MissionNotClearedTexture);	
	
	if (MidBossWinAchievementImage)
		MidBossWinAchievementImage->SetBrushFromTexture(CurrentStageMissions[2] ? MissionClearedTexture : MissionNotClearedTexture);
}
