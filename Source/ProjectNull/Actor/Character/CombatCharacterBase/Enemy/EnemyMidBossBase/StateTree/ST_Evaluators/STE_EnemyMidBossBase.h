#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyAttackData/EnemyAttackDataAsset.h"
#include "STE_EnemyMidBossBase.generated.h"

/** 中ボスクラス */
class AEnemyMidBossBase;

/** 敵のランタイムデータクラス */
class UEnemyMidBossRuntimeData;

UCLASS()
class PROJECTNULL_API USTE_EnemyMidBossBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief ステートツリーの開始処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStart(FStateTreeExecutionContext& InContext)						override;

	/**
	 * @brief ステートツリーの更新処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 * @param a_DeltaTime デルタタイム
	 */
	virtual void Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)	override;

	/**
	 * @brief ステートツリーの終了処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStop(FStateTreeExecutionContext& InContext)						override;

	/**
	 * @brief デリゲートへの関数登録
	 */
	virtual void RegisterDelegate();

public:

	//~ Begin Setters
	/**
	 * @brief 移動方向のセット
	 * @param a_MoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& InMoveDir)
	{
		MoveDir = InMoveDir;
	}

	/**
	 * @brief ターゲット座標のセット
	 * @param InTargetLocation ターゲットの座標
	 */
	virtual void SetTargetLocation(const FVector& InTargetLocation)
	{
		TargetLocation = InTargetLocation;
	}

	/**
	 * @brief 最大体力に対するダメージ割合のセット
	 * @param InDamageRatio 計算後のダメージ割合
	 */
	virtual void SetDamageRatio(float InDamageRatio)
	{
		DamageRatio = InDamageRatio;
	}

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param InDistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float InDistSqr)
	{
		TargetDistanceSqr = InDistSqr;
	}

	/**
	 * @brief ステートEnumをセット
	 * @param InTargetState 切り替え先ステート
	 */
	virtual void SetEnemyState(EEnemyState InTargetState)
	{
		EnemyState = InTargetState;
	}

	/**
	 * @brief 発動したい攻撃データをセット
	 * @param InAttackType 発動したい攻撃データ
	 */
	virtual void SetAttackData(const FEnemyAttackData& InAttackData)
	{
		AttackData = InAttackData;
	}

	//~ End Setters

private:

	/** 持ち主 */
	UPROPERTY()
	TObjectPtr<AEnemyMidBossBase>	OwnerEnemy = nullptr;

	/** 敵のランタイムデータを管理するクラス */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowAbstract = "true"))
	TObjectPtr<UEnemyMidBossRuntimeData>	EnemyRuntimeData;

	/** 移動ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	FVector MoveDir = FVector::ZeroVector;

	/** ターゲットの座標 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float TargetDistanceSqr = 0.f;

	/**	最大体力に対する受けたダメージの割合 */
	UPROPERTY(VisibleAnywhere, Category = "OutPut", meta = (AllowPrivateAcces = "true"))
	float DamageRatio = 1.f;

	/**	ステートEnum */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::Idle;
	
	/**	発動したい攻撃データ */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	FEnemyAttackData AttackData = FEnemyAttackData();
};
