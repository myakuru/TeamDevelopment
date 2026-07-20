
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "Laserbeam.generated.h"

class UBoxComponent;
class UEffectBase;
class UPlayerRuntimeData;
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

	void SetLaserEnabled(bool bEnabled);

private:

	UFUNCTION()
	void OnLaserbeamBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UEffectBase>>	NiagaraEffectArray;
	
	/** プレイヤーのRuntimeDataクラス */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;
	
	UPROPERTY(EditAnywhere)
	float AttackPower;
};
