#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../EnemyDataStruct.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyBase;

UCLASS()
class UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	virtual void NativeUpdateAnimation(float DeltaTime)override;

	// AnimBPから参照する変数
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EEnemyState CurrentState = EEnemyState::Spawn;

private:

	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;
};