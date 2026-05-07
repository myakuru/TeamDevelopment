#pragma once

class AEnemyBase;
class UEnemyDataAsset;
class UEnemyPoolConfig;

#include "Subsystems/WorldSubsystem.h"
#include "EnemyPoolSubsystem.generated.h"

/**
* データアセット1つにつき1つのプール
*/
USTRUCT()
struct FEnemyPool
{
	GENERATED_BODY()

	/** 待機中のActor*/
	UPROPERTY()
	TArray<TObjectPtr<AEnemyBase>> Inactive;

	/** 稼働中のActor*/
	UPROPERTY()
	TArray<TObjectPtr<AEnemyBase>> Active;
};



/**
* 敵のオブジェクトプールを管理するWorldSubSystem
* GetWorld->GetSubSystem<UEnemyPoolSubSystem>()でどこからでもアクセス可
*/
UCLASS()
class PROJECTNULL_API UEnemyPoolSubSystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	/**
	* 起動時にPoolを事前作成する
	* EnemySpawnerのBeginPlayで呼ぶ
	*/

	/**
	 * @brief 
	 * @param InData 
	 */
	void WarmUp(UEnemyPoolConfig* InData);

	/**
	* Poolから敵を取り出してActivateする
	* EnemySpawnerのSpawnEnemy内で呼ぶ
	*/
	AEnemyBase* Spawn(UEnemyPoolConfig* InData, const FVector& Location);

	/**
	* 敵をPoolに返却する
	* FEnemyStateDead::OnUpdateから呼ぶ
	*/
	void Return(AEnemyBase* Enemy);

private:

	AEnemyBase* CreateNewEnemy(UEnemyPoolConfig* InData);

	/** DataAssetのPrimaryAssetIdをキーにPool管理*/
	UPROPERTY()
	TMap<FPrimaryAssetId, FEnemyPool> Pools;

	/** 返却時にどのPoolか逆引きするマップ*/
	UPROPERTY()
	TMap<AEnemyBase*, UEnemyPoolConfig*> EnemyToConfig;

};