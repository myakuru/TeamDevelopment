
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Laserbeam.generated.h"

class UBoxComponent;


UCLASS(Blueprintable)
class PROJECTNULL_API ALaserbeam : public AActor
{
	GENERATED_BODY()	
public:	
	ALaserbeam();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;
	void SetCollisionEnabled(const ECollisionEnabled::Type CollisionType);
private:

};
