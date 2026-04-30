#pragma once

class AEnemyBase;
class UEnemyDataAsset;

#include "Engine/DataAsset.h"
#include "EnemyPoolConfig.generated.h"

UCLASS(BlueprintType)
class UEnemyPoolConfig : public UDataAsset
{
	GENERATED_BODY()

public:

	/** プールで生成する際に利用するクラス*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool")
	TSubclassOf<AEnemyBase> EnemyClass;

	/** 
	* プールであらかじめ確保する数
	* メモリを使いすぎないために、数は適宜調整
	*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool")
	int32 PoolSize = 50;

	/** 生成するときに使用するエネミーのデータ*/
	/*UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool")
	TSubclassOf<UEnemyDataAsset> EnemyData;*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pool")
	TObjectPtr<UEnemyDataAsset> EnemyData;
};