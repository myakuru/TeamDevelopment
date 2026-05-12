#pragma once

#include "CoreMinimal.h"
#include "EnemyDataStruct.generated.h"

// アニメーション用Enum
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Spawn UMETA(DisplayName = "生成"),
	Chase UMETA(DisplayName = "追跡"),
	Attack UMETA(DisplayName = "攻撃"),
	Dead UMETA(DisplayName = "死亡"),
};

/// <summary>
/// 敵ステータスのスケーリング情報
/// </summary>
USTRUCT(BlueprintType)
struct FStatScaling
{
	GENERATED_BODY()

public:
	// 基礎数値
	UPROPERTY(EditAnywhere)
	int32 Base = 100;

	// 倍率
	UPROPERTY(EditAnywhere)
	float Scale = 1.0f;

	// 倍率増加量
	UPROPERTY(EditAnywhere)
	float ScalePerKill = 0.005f;

	/// <summary>
	/// 基礎数値 * 倍率
	/// </summary>
	/// <returns>最終的な数値を返す</returns>
	int32 GetFinalValue(int32 Count)
	{
		Scale = 1.0f + Count * ScalePerKill;
		return static_cast<int32>(Base * Scale);
	}
};

/// <summary>
/// 敵基本ステータス
/// </summary>
USTRUCT(BlueprintType)
struct FEnemyStatus
{
	GENERATED_BODY()

public:
	// 移動方向
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	// 回転補間速度
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;

	// 最終的なヒットポイント
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	// スケーリング計算用ヒットポイント
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	// 最終的な攻撃力
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

	// スケーリング計算用攻撃力
	UPROPERTY(EditAnywhere)
	FStatScaling AttackScaling;

	// エネミーの重量
	UPROPERTY(EditAnywhere)
	float	KnockBackWeight = 1.0f;

	// ノックバック方向
	FVector KNockBackVelocity = FVector::ZeroVector;

	// エネミーが吹き飛び中の判定フラグ
	bool	KnockBackFlg = false;

	// 経験値
	UPROPERTY(EditAnywhere)
	float Exp = 0;

	// ギアエネルギー
	UPROPERTY(EditAnywhere)
	float GearEnergy = 0;

	// ターゲットとの簡易距離
	float TargetDistanceSqr = 0.0f;

	// 攻撃可能距離
	UPROPERTY(EditAnywhere)
	float AttackDistance = 20.0f;

	// 攻撃可能フラグ
	bool CanAttack = false;

	/** 生存フラグ*/
	bool IsAlive = true;

	// 死んだときのパーティクルの色
	UPROPERTY(EditAnywhere, Category = "Experience")
	FLinearColor ExpColor = FLinearColor::Blue;

	// パーティクルのサイズ
	UPROPERTY(EditAnywhere, Category = "Experience")
	float ExpSize = 1.0f;
};