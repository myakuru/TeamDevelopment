#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

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

};
