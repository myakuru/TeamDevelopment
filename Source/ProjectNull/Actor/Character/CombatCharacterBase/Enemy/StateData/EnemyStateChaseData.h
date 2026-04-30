#pragma once

/** 追跡Stateの固有データ*/

#include <ProjectNull/Actor/Character/StateData/StateDataBase.h>
#include "EnemyStateChaseData.generated.h"

UCLASS(BlueprintType)
class PROJECTNULL_API UEnemyStateChaseData : public UStateDataBase
{
	GENERATED_BODY()

public:

	/** 仮データ*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float kariData = 0.0f;

};