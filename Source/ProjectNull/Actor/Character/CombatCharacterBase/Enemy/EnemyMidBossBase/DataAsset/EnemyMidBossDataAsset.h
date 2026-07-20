#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataAsset.h"
#include "EnemyMidBossDataAsset.generated.h"

/**
 * 中ボスに必要なデータアセット
 */
UCLASS()
class PROJECTNULL_API UEnemyMidBossDataAsset : public UEnemyDataAsset
{
	GENERATED_BODY()
	
public:

	/**	攻撃のインターバル */
	UPROPERTY(EditAnywhere)
	float AttackInterval = 1.f;
};
