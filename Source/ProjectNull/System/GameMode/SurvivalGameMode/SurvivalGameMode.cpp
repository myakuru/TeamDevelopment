
#include "SurvivalGameMode.h"

#include "EngineUtils.h" 
#include "CineCameraActor.h"
#include "CineCameraComponent.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	// PlayerController の初期化を待って次フレームで再生
	GetWorldTimerManager().SetTimerForNextTick
	(
		this, &ASurvivalGameMode::PlayOpeningCutscene
	);
}

void ASurvivalGameMode::PlayOpeningCutscene()
{
	// シーケンスが未設定ならそのままゲーム開始
	if (!OpeningSequence) return;

	// 入力をブロック
	if (ARobotController* RC = Cast<ARobotController>(GetWorld()->GetFirstPlayerController()))
	{
		RC->SetCanReceiveInput(false);
	}

	ALevelSequenceActor* OutActor = nullptr;

	// レベルシーケンスを再生
	FMovieSceneSequencePlaybackSettings Settings;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
	(
		GetWorld(), OpeningSequence, Settings, OutActor
	);

	if (SequencePlayer)
	{
		// 再生終了時に OnOpeningCutsceneFinished を呼ぶ
		SequencePlayer->OnFinished.AddDynamic
		(
			this, &ASurvivalGameMode::OnOpeningCutsceneFinished
		);
		SequencePlayer->Play();
	}
}

void ASurvivalGameMode::OnOpeningCutsceneFinished()
{
	// 入力を解放してゲーム開始
	if (ARobotController* RC = Cast<ARobotController>(GetWorld()->GetFirstPlayerController()))
	{
		RC->SetCanReceiveInput(true);
	}
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
	// 敵管理クラスの更新メソッドを呼ぶ（毎フレーム）
	if (enemyManager) {
		//(LogTemp, Warning, TEXT("num %d"), enemyManager->GetEnemyNum());
		enemyManager->UpdateEnemies(DeltaTime);
	}

	/** アイテム管理クラスの情報取得*/
	UItemManagerSubsystem* itemManager = GetWorld()->GetSubsystem<UItemManagerSubsystem>();

	/** 更新*/
	if (itemManager)
	{
		itemManager->UpdateItemManagers(DeltaTime);
	}
}
