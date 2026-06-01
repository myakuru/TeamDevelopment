
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>

#include "CrossLaserAttack.generated.h"


class UBoxComponent;

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECTNULL_API UCrossLaserAttack final : public UAttackBase
{
	GENERATED_BODY()
public:
	UCrossLaserAttack();
public:
	void Execute()override;
	void Update(
		float DeltaTime,
		AActor* Player = nullptr,
		UEnemyManagerSubsystem* EnemyManager = nullptr)override;

	void End();

private:

	/** 現在の角度 */
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	/** 回転速度(度/秒) */
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	UPROPERTY(EditAnywhere,Instanced)
	TArray<TObjectPtr<UBoxComponent>> LaserBoxes;
};
