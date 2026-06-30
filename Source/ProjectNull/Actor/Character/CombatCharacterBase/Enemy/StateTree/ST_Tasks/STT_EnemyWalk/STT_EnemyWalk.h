// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyWalk.generated.h"

/**
 * 持ち主クラス
 */
class AEnemyBase;

/**
 * 敵の移動ステート
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyWalk : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTT_EnemyWalk(const FObjectInitializer& a_ObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;

private:

	/**
	 * @brief 移動に必要なパラメータの初期化
	 */
	void InitializeWalkParams();

	/**
	 * @brief 移動処理
	 * @param a_DeltaTime デルタタイム
	 */
	void Move(const float a_DeltaTime);

	/**
	 * @brief 坂道移動処理
	 * @param a_HitResult HitResult
	 */
	void MoveSlope(const FHitResult& a_HitResult);

	/**
	 * @brief 段差を超える処理
	 * @param a_HitResult	HitResult
	 * @param a_MoveVec		移動量
	 * @param a_HeightDiff	段差との高さの差
	 */
	void MoveStep(const FHitResult& a_HitResult, const FVector& a_MoveVec,const float a_HeightDiff);

	/**
	 * @brief 衝突時に使われなかったベクトルを算出
	 * @param a_DeltaMoveDir 移動量
	 * @param a_HitTime Hitするまでの時間(FHitResultのメンバから取ってくる)
	 * @return 衝突時に使われなかったベクトル
	 */
	FVector CaluculateRemainingMoveDir(const FVector& a_DeltaMoveDir, const float a_HitTime);

	/**
	 * @brief 回転補間処理
	 * @param a_CurrentRot	現在の角度
	 * @param a_TargetRot	目標角度
	 * @param a_RotSpeed	回転スピード
	 * @param a_DeltaTime	デルタタイム
	 * @return 補間結果
	 */
	FRotator CalculateRotationToMoveDir(const FRotator& a_CurrentRot, const FRotator& a_TargetRot, float a_RotSpeed, float a_DeltaTime);

private:

	/** 持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;

	/** 移動ベクトル */
	UPROPERTY(EditAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

	/**	重力の速度 */
	UPROPERTY()
	FVector GravityVelocity = FVector::ZeroVector;

	/** 以前触れた地面の法線 */
	UPROPERTY()
	FVector AfterGroundNormal = FVector::ZeroVector;

	/**	移動速度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float MoveSpeed = 600.f;

	/**	回転速度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float RotationInterpSpeed = 5.f;

	/**	歩くことのできる床の角度 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float WarkableFloorAngle = 45.f;

	/**	超える事の出来る段差の高さ */
	UPROPERTY(EditAnywhere, Category = "Input")
	float MaxStepHeight = 100.f;

	/**	カプセルの高さ半径(高さ補正で使う) */
	UPROPERTY(EditAnywhere, Category = "Input")
	float CapsuleHalfHeight = 88.f;

	/**	接地フラグ */
	UPROPERTY()
	bool isGround = false;
};
