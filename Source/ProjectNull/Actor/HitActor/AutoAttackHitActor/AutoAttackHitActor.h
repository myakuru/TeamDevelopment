
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "AutoAttackHitActor.generated.h"

class UBoxComponent;
class USuperGameInstance;
class UPlayerRuntimeData;

UCLASS()
class PROJECTNULL_API AAutoAttackHitActor : public AActor
{
	GENERATED_BODY()
	
public:	
	AAutoAttackHitActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void SetHitEnabled(bool bInEnabled);

private:

	UFUNCTION()
	void OnAutoAttackBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);


	void PerformHitSweep();

	float SetAttackDamage();


	UPROPERTY()
	TSet<TObjectPtr<AActor>> HitActors;

	FVector PreviousLocation;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UBoxComponent> BoxComp;

	/** ゲームインスタンスへの参照 */
	UPROPERTY()
	TObjectPtr<USuperGameInstance> GameInstance;

	/** プレイヤーのランタイムデータへの参照 */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;

	bool bEnabled;
};
