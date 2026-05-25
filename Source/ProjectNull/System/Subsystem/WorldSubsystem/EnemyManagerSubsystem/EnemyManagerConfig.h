#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyManagerConfig.generated.h"

UCLASS(BlueprintType)
class PROJECTNULL_API UEnemyManagerConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	/** Updateの回数を制限する際に利用する距離のデータ*/
	UPROPERTY(EditAnywhere)
	TArray<float> UpdateLimitDistance;
	
};
