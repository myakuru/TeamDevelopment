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

	/** レベルに配置したシネカメラ（エディタで設定） */
	UPROPERTY()
	TObjectPtr<AMyCineCameraActor> OpeningCameraActor;

};
