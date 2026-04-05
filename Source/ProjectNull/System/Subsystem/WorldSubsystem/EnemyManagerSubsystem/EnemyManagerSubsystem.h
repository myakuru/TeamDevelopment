// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerSubsystem.generated.h"

//　敵の中間基底クラス
class AEnemyBase;

/// <summary>
/// 敵管理クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UEnemyManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

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

private:

	/// <summary>
	/// 敵をまとめるリスト
	/// </summary>
	UPROPERTY()
	TArray<AEnemyBase*> EnemyGruntList;
};
