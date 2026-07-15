// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerSubsystem.generated.h"

// 敵の中間基底クラス
class AEnemyBase;

class AEnemyISMManager;

class UEnemyISMManagerConfig;
class UEnemyManagerConfig;

class USoundManager;

/// <summary>
/// 敵管理クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UEnemyManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	/// <summary>
	/// 敵の更新処理
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	void UpdateEnemies(float DeltaTime);

	/// <summary>
	/// 敵のリストへ対象を登録
	/// </summary>
	/// <param name="enemy">敵のポインタ</param>
	void RegisterEnemy(AEnemyBase* Enemy);

	/// <summary>
	/// 敵のリストから対象を削除
	/// </summary>
	/// <param name="enemy">敵のポインタ</param>
	void RemoveEnemy(AEnemyBase* Enemy);

	/// <summary>
	/// リスト内の敵数
	/// </summary>
	/// <returns>敵数</returns>
	int32 GetEnemyNum() const { return EnemyGruntList.Num(); }

	/// <summary>
	/// 敵をまとめるリスト
	/// </summary>
	TArray<AEnemyBase*> GetEnemyList() const { return EnemyGruntList; }

	/** ISMManager登録
	*	ISMManagerが自己登録するための関数
	*/
	void RegisterISMManager(AEnemyISMManager* Manager);

	/**
	*	指定したクラスのISMManagerを返す
	*	EnemyBaseのBeginPlayでManagerを探すための関数
	*/
	AEnemyISMManager* GetISMManager(TSubclassOf<AEnemyISMManager> ManagerClass) const;

	/**
	 * ダメージ効果音を返す
	 */
	USoundBase* GetDamagedSound() const;
	
	/** すべての敵を破壊する*/
	void DestroyAllEnemy();

private:

	/**
	* 距離を算出する
	*/
	float CalcDistance(const FVector& EnemyPos, const FVector& PlayerPos);

	/**
	* 計算回数を算出する
	*/
	int32 CalcInterval(float Distance);

	/// <summary>
	/// 敵をまとめるリスト
	/// </summary>
	UPROPERTY()
	TArray<TObjectPtr<AEnemyBase>> EnemyGruntList;

	/** 
	* ISMManager管理
	* 種類ごとのISMManagerをクラスをキーに管理する
	* GetISMManagerでクラスを指定して対応するISSMManagerを返す
	*/
	UPROPERTY()
	TMap<UClass*, TObjectPtr<AEnemyISMManager>> ISMManagerMap;

	/** ISMManagerの一覧を持つDataAssetの参照*/
	UPROPERTY()
	TSoftObjectPtr<UEnemyISMManagerConfig> ISMManagerConfig;

	UPROPERTY()
	TObjectPtr<UEnemyManagerConfig> ManagerConfig;

	/**
	* フレーム数（Updateの回数を指定するときに使う）
	*/
	int32 FrameCount = 0;
};