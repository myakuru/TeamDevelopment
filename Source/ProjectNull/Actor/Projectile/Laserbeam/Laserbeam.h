
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Laserbeam.generated.h"

class UBoxComponent;
class UEffectBase;


UCLASS()
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

	UFUNCTION()
	void OnLaserBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY()
	TObjectPtr<UBoxComponent>	BoxComp;

	UPROPERTY(EditAnywhere,Instanced)
	TObjectPtr<UEffectBase>		NiagaraEffect;

	
};
