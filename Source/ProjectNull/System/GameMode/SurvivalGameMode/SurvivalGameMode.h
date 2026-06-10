#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

class AMyCineCameraActor;

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

	UFUNCTION()
	void OnCutsceneFinished();

	/** レベルに配置したシネカメラ（エディタで設定） */
	UPROPERTY()
	TObjectPtr<AMyCineCameraActor> OpeningCameraActor;

	UPROPERTY(EditAnywhere, Category = "Opening")
	FVector PlayerLocation = FVector(0.0f, 0.0f, 1000.0f);

	/** ビュー切り替え時のブレンド速度 */
	UPROPERTY(EditAnywhere, Category = "Opening")
	float TargetBlendSpeed = 0.0f;

};
