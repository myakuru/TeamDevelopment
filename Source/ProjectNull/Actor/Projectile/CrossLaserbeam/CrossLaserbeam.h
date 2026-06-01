
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CrossLaserbeam.generated.h"


class UBoxComponent;

UCLASS(Blueprintable)
class PROJECTNULL_API ACrossLaserbeam : public AActor
{
	GENERATED_BODY()
	
public:	
	ACrossLaserbeam();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;


private:
	
	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UBoxComponent>> LaserBoxes;

	UPROPERTY(EditAnywhere)
	TArray<FTransform> LaserRelativeTransforms;
};
