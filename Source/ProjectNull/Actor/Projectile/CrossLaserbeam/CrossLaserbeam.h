
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "CrossLaserbeam.generated.h"


/** 敵の中間基底クラス */
class AEnemyBase;

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
	
	UFUNCTION()
	void OnLaserEndOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	void OnHit();


	UPROPERTY(EditAnywhere)
	TArray<TObjectPtr<UBoxComponent>> LaserBoxes;

	UPROPERTY()
	TSet<TWeakObjectPtr<AEnemyBase>> HitEnemies;

	FTimerHandle HitIntervalTimerHandle;

	UPROPERTY(EditAnywhere)
	float HitInterval;
};
