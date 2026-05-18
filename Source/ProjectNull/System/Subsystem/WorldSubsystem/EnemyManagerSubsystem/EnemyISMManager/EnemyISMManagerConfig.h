#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyISMManagerConfig.generated.h"

class AEnemyISMManager;

UCLASS(BlueprintType)
class PROJECTNULL_API UEnemyISMManagerConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	/** スポーンするISMManagerのクラス一覧*/
	UPROPERTY(EditDefaultsOnly, Category = "ISM")
	TArray<TSubclassOf<AEnemyISMManager>> ISMManagerClasses;
};
