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
	virtual void SetMoveDir(const FVector& a_MoveDir) { MoveDir = a_MoveDir; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param a_DistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float a_DistSqr) { TargetDistanceSqr = a_DistSqr; }

	/**
	 * @brief ステートEnumをセット
	 * @param a_TargetState 切り替え先ステート
	 */
	virtual void SetEnemyState(EEnemyState a_TargetState) { EnemyState = a_TargetState; }

	/**
	* @brief 生存状態のセット
	* @param IsAliveがtrueで生存
	*/
	virtual void SetIsAlive(bool a_IsAlive) { IsAlive = a_IsAlive; }

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

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float TargetDistanceSqr = 0.0f;
	
	/** ノックバック時の重さ */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float KnockBackWeight = 0.0f;

	/**	移動速度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float MoveSpeed = 0.0f;

	/**	回転速度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float RotateSpeed = 0.0f;

	/**	歩くことのできる斜面角度 */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float WalkableFloorAngle = 0.0f;

	/**	超える事の出来る段差の最大の高さ */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float MaxStepHeight = 0.0f;

	/**	カプセルの高さ半径(高さ補正で使う) */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	float CapsuleHalfHeight = 0.0f;

	/**	ステートEnum */
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	EEnemyState EnemyState = EEnemyState::None;

	/** 死亡判定フラグ*/
	UPROPERTY(VisibleAnywhere, Category = "Output", meta = (AllowPrivateAccess = "true"))
	bool IsAlive = true;
};
