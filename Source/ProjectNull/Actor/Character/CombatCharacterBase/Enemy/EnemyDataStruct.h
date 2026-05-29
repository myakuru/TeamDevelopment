#pragma once

#include "CoreMinimal.h"
#include "EnemyDataStruct.generated.h"

// アニメーション用Enum
UENUM(BlueprintType)
enum class EEnemyState : uint8
{
	Idle		UMETA(DisplayName = "棒立ち"),
	Walk		UMETA(DisplayName = "移動"),
	Attack		UMETA(DisplayName = "攻撃"),
	Hit			UMETA(DisplayName = "被弾"),
	Death		UMETA(DisplayName = "死亡"),
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

	// スケーリング計算用ヒットポイント
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	// スケーリング計算用攻撃力
	UPROPERTY(EditAnywhere)
	FStatScaling AttackScaling;

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

	/**	歩くことのできる地面の角度 */
	UPROPERTY(EditAnywhere)
	float WalkableFloorAngle = 0.0f;

	/**	超える事の出来る段差の高さ */
	UPROPERTY(EditAnywhere)
	float MaxStepHeight = 0.0f;

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

	/**	ステートタグ */
	UPROPERTY(EditAnywhere, Category = "StateTag")
	EEnemyState StateTag = EEnemyState::None;

	/** アニメーションブレンドにかける時間*/
	UPROPERTY(EditAnywhere, Category = "AnimBlend")
	float BlendSpeed = 1.2f;
	
	/** 生存フラグ*/
	UPROPERTY()
	bool IsAlive = true;
};