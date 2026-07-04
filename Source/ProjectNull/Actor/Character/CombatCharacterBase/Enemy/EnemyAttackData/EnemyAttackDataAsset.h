#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ProjectNull/System/Combat/Attack/AttackBase.h"
#include "ProjectNull/System/Combat/CombatTypes/CombatTypes.h"
#include "EnemyAttackDataAsset.generated.h"

/** 攻撃に使用する要素 */
USTRUCT(BlueprintType)
struct FEnemyAttackData
{
	GENERATED_BODY()

	/** 攻撃の種類(実行時のキーとしても使用) */
	UPROPERTY(EditAnywhere)
	EEnemyAttackType AttackType = EEnemyAttackType::Attack1;

	/**	攻撃可能最大距離 */
	UPROPERTY(EditAnywhere)
	float MaxDistance = 0.f;

	/**	攻撃クラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UAttackBase> AttackClass;

	/**	攻撃に適応したアニメーション */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UAnimMontage> Animation;

	/**
	 * @brief オペレーターを使って比較
	 * @param InAttackData 比較対象
	 * @return 一緒ならtrue
	 */
	bool operator==(const FEnemyAttackData& InAttackData)const 
	{
		return (
			InAttackData.Animation == Animation &&
			InAttackData.AttackClass == AttackClass &&
			InAttackData.MaxDistance == MaxDistance
			);
	}
};

/**
 * 敵の攻撃を管理するデータアセット
 */
UCLASS()
class PROJECTNULL_API UEnemyAttackDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/**	敵が持つ攻撃データ配列 */
	UPROPERTY(EditAnywhere)
	TArray<FEnemyAttackData> AttackDatas;

};
