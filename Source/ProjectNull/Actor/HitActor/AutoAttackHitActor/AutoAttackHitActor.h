
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

	void PerformHitSweep();

	float GetAttackDamage() const;


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
	
	/** 自動攻撃の攻撃力係数 */
	UPROPERTY(EditAnywhere)
	float AttackPowerScale;

	bool bEnabled;
};
