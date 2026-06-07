#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossDataAsset.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyBossAttackSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** このボスが使う攻撃パターン一覧 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FBossAttackPattern> Patterns;
};
