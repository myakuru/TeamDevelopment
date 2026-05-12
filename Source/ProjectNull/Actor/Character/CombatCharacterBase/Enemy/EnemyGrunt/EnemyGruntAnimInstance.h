#pragma once

#include "CoreMinimal.h"
#include "EnemyGruntAnimInstance.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyGruntAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// 移動しているフラグ
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AnimState")
	bool bIsMoving;

};