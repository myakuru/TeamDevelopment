#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureBoxSpawner.generated.h"

UCLASS()
class PROJECTNULL_API ATreasureBoxSpawner : public AActor
{
	GENERATED_BODY()
	
public:
	ATreasureBoxSpawner();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

};
