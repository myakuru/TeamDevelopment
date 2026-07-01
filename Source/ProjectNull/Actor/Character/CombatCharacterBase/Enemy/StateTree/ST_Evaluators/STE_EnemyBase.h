#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeEvaluatorBlueprintBase.h"
#include "../../EnemyDataStruct.h"
#include "STE_EnemyBase.generated.h"

/** 敵クラス */
class AEnemyBase;

/** 敵のランタイムデータクラス */
class UEnemyRuntimeData;

/**
 * 敵のステートツリーの基底クラス
 */
UCLASS()
class PROJECTNULL_API USTE_EnemyBase : public UStateTreeEvaluatorBlueprintBase
{
	GENERATED_BODY()
	
public:

	/**
	 * @brief ステートツリーの開始処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStart(FStateTreeExecutionContext& a_Context)						override;
	
	/**
	 * @brief ステートツリーの更新処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 * @param a_DeltaTime デルタタイム
	 */
	virtual void Tick(FStateTreeExecutionContext& a_Context,const float a_DeltaTime)	override;
	
	/**
	 * @brief ステートツリーの終了処理
	 * @param a_Context 持ち主のコンテキスト構造体
	 */
	virtual void TreeStop(FStateTreeExecutionContext& a_Context)						override;

	/**
	 * @brief 固定パラメーターの登録
	 */
	virtual void RegisterFixedParams();

	/**
	 * @brief デリゲートへの関数登録
	 */
	virtual void RegisterDelegate();

public:

	//~ Begin Setter
	
	/**
	 * @brief 移動方向のセット
	 * @param a_MoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& InMoveDir) 
	{ MoveDir = InMoveDir; }

	/**
	 * @brief ターゲット座標のセット
	 * @param InTargetLocation ターゲットの座標
	 */
	virtual void SetTargetLocation(const FVector& InTargetLocation) 
	{ TargetLocation = InTargetLocation; }

	/**
	 * @brief 最大体力に対するダメージ割合のセット
	 * @param InDamageRatio 計算後のダメージ割合
	 */
	virtual void SetDamageRatio(float InDamageRatio) 
	{ DamageRatio = InDamageRatio; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param a_DistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float InDistSqr) 
	{ TargetDistanceSqr = InDistSqr; }

	/**
	 * @brief ステートEnumをセット
	 * @param a_TargetState 切り替え先ステート
	 */
	virtual void SetEnemyState(EEnemyState InTargetState)
	{ EnemyState = InTargetState; }

	/**
	* @brief 生存状態のセット
	* @param IsAliveがtrueで生存
	*/
	virtual void SetIsAlive(bool InIsAlive) 
	{ IsAlive = InIsAlive; }

	//~ End Setter

private:

	/** 持ち主 */
	UPROPERTY()
	TObjectPtr<AEnemyBase>	OwnerEnemy = nullptr;

	/** 敵のランタイムデータを管理するクラス */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowAbstract = "true"))
	TObjectPtr<UEnemyRuntimeData>	EnemyRuntimeData;

	/** 移動ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "Output",meta = (AllowPrivateAccess = "true"))
	FVector MoveDir = FVector::ZeroVector;

	/** ターゲットの座標 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	FVector TargetLocation = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float TargetDistanceSqr = 0.f;
	
	/**	最大体力に対する受けたダメージの割合 */
	UPROPERTY(VisibleAnywhere, Category = "OutPut",meta=(AllowPrivateAcces="true"))
	float DamageRatio = 1.f;

	/** ノックバック時の重さ */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float KnockBackWeight = 0.f;

	/**	移動速度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 600.f;

	/**	回転速度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float RotateInterpSpeed = 5.f;

	/**	歩くことのできる斜面角度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float WalkableFloorAngle = 45.f;

	/**	超える事の出来る段差の最大の高さ */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float MaxStepHeight = 5.f;

	/**	カプセルの高さ半径(高さ補正で使う) */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float CapsuleHalfHeight = 88.f;

	/**	ステートEnum */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::Idle;

	/** 死亡判定フラグ*/
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	bool IsAlive = true;
};
