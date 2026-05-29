#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class ACineCameraActor;
class APlayerBase;

/**
 * defaultのゲームモード
 */
UCLASS()
class PROJECTNULL_API ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:

	ASurvivalGameMode();

	virtual void Tick(float DeltaTime) override;

protected:

	void BeginPlay() override;

private:
	/** オープニングで再生するレベルシーケンス（エディタで設定） */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TObjectPtr<ULevelSequence> OpeningSequence;

	/** レベルに配置したシネカメラ（エディタで設定） */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TObjectPtr<ACineCameraActor> OpeningCameraActor;

	void PlayOpeningCutscene();

	/** シーケンス終了時に呼ばれる（DynamicDelegate に登録するため UFUNCTION が必要） */
	UFUNCTION()
	void OnOpeningCutsceneFinished();

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;

	TObjectPtr<APlayerBase> PlayerInstance;

};
