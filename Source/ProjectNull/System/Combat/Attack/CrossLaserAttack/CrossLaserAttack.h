
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>

#include "CrossLaserAttack.generated.h"


class ACrossLaserbeam;

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UCrossLaserAttack final : public UAttackBase
{
	GENERATED_BODY()
public:
	UCrossLaserAttack();
public:
	void Initialize(class AActor* Owner) override;
	void Execute()override;
	void Update(
		float DeltaTime,
		AActor* Player = nullptr,
		UEnemyManagerSubsystem* EnemyManager = nullptr)override;

	void End();

private:

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ACrossLaserbeam> CrossLaserbeam;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACrossLaserbeam> CrossLaserbeamClass;

	/** 現在の角度 */
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	/** 回転速度(度/秒) */
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

};
