#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

class ADamageNumberActor;

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

	UPROPERTY(EditDefaultsOnly, Category = "Damage UI")
	TSubclassOf<ADamageNumberActor> DamageNumberActorClass;

	UPROPERTY(EditDefaultsOnly, Category = "Damage UI")
	int32 DamageNumberPoolSize = 300;

};
