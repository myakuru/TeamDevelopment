
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CrossLaserbeam.generated.h"


class ALaserbeam;

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

	void SetLaserEnabled(bool bEnabled);
	
private:

	UPROPERTY(EditAnywhere,Instanced)
	TArray<TObjectPtr<ALaserbeam>> Laserbeams;

};
