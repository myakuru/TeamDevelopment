
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CrossLaserbeam.generated.h"

class UBoxComponent;
class UEffectBase;

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


	UFUNCTION()
	void OnLaserBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	static const int32 LaserbeamNum = 4;

	UPROPERTY(EditAnywhere,Instanced)
	TArray<TObjectPtr<UBoxComponent>> BoxCompArray;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UEffectBase>>	NiagaraEffectArray;
	

};
