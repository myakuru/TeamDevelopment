#pragma once

#include "EnemyBossDataStruct.generated.h"

class UAnimMontage;

UENUM(BlueprintType)
enum class EBossActionType : uint8
{
	None,
	PlayAttack,			// 近接攻撃を出す
	Strafe,				// プレイヤーの周りを回って様子見
	ApproachWalk,		// 歩いて近づく
	ApproachRun,		// 走って近づく
	JumpAttack,			// 跳びかかる
	RangedAttack		// 遠距離攻撃
};

USTRUCT(BlueprintType)
struct FBossAttackPattern
{
	GENERATED_BODY()

	/** 攻撃パターン名*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Name;

	/** 攻撃パターンEnum*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBossActionType ActionType = EBossActionType::None;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange = 300.0f;

	/** アニメーションモンタージュ、単発なら要素1、連撃なら複数*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
};

/// <summary>
/// 敵基本ステータス
/// </summary>
USTRUCT(BlueprintType)
struct FBossEnemyStatus
{
	GENERATED_BODY()

public:

	// 移動方向
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	// ノックバック方向
	UPROPERTY()
	FVector KNockBackVelocity = FVector::ZeroVector;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	// 回転補間速度
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;

	// エネミーの重量
	UPROPERTY(EditAnywhere)
	float	KnockBackWeight = 1.0f;

	// 経験値
	UPROPERTY(EditAnywhere)
	float Exp = 0;

	// ギアエネルギー
	UPROPERTY(EditAnywhere)
	float GearEnergy = 0;

	// ターゲットとの簡易距離
	UPROPERTY()
	float TargetDistanceSqr = 0.0f;

	// 攻撃可能距離
	UPROPERTY(EditAnywhere)
	float AttackDistance = 20.0f;

	// パーティクルのサイズ
	UPROPERTY(EditAnywhere, Category = "Experience")
	float ExpSize = 1.0f;

	// 死んだときのパーティクルの色
	UPROPERTY(EditAnywhere, Category = "Experience")
	FLinearColor ExpColor = FLinearColor::Blue;

	// 最終的なヒットポイント
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	// 最終的な攻撃力
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

	/** 生存フラグ*/
	UPROPERTY()
	bool IsAlive = true;
};
