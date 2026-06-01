#pragma once

#include "CoreMinimal.h"

#include "../AttackBase.h"

#include "FanAttackBase.generated.h"

/** 扇状攻撃基底クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFanAttackBase : public UAttackBase
{
	GENERATED_BODY()
public:
	UFanAttackBase();
public:

	/**
	 * @brief 開始の際の初期化
	 */
	virtual void Start();

	virtual void Execute()override;
	
	virtual void Update(
		float DeltaTime,
		AActor* Player = nullptr,
		UEnemyManagerSubsystem* EnemyManager = nullptr)override;

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 * @return 更新結果
	 */
	virtual bool UpdateAttack(float DeltaTime);

	bool CanDeactivate();

	bool IsActiveFirstFrame();

	void UpdatePrevActiveFlg();

	/**
	 * @brief 攻撃範囲内にターゲットがいるかどうかの判定
	 * @param Target ターゲット
	 * @return ターゲットが攻撃範囲内かどうか
	 */
	virtual bool IsTargetInRange(AActor* Target) override;

	/**
	 * @brief 攻撃方向の計算
	 * @param forwardVector 前方方向
	 * @return 計算結果
	 */
	virtual FVector CalcAttackDir(const FVector& forwardVector) const override;
	FVector CalcAttackDir(const FVector& forwardVector,float Angle) const;

	/**
	 * @brief 半径の二乗を取得
	 * @return 半径の二乗
	 */
	inline float GetRadiusSquared() const { return Radius * Radius; }

	/**
	 * @brief 扇角のcos値
	 * @return 扇角のcos値
	 */
	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

protected:

	/**
	 * @brief 敵リストに対する攻撃判定
	 * @param EnemyManager 敵管理クラスのアドレス
	 */
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) override;

	
	/**
	 * @brief プレイヤーに対する攻撃判定
	 * @param Player プレイヤークラス
	 */
	virtual void AttackJudgePlayer(AActor* Player) override;

public:

	/** 攻撃の持続時間(秒) */
	UPROPERTY(EditAnywhere)
	float Duration;

	/** 経過時間 */
	float ElapsedTime;

	/** 回転するかどうか */
	UPROPERTY(EditAnywhere)
	bool bRotate;

	/** 回転速度(度/秒) */
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	/** 攻撃半径 */
	UPROPERTY(EditAnywhere)
	float Radius;

	/** 扇の広さ(角度) */
	UPROPERTY(EditAnywhere)
	float ConeAngle;

	/** 全フレームでアクティブだったか */
	bool bPrevActive;

	/** 開始の角度 */
	UPROPERTY(EditAnywhere)
	float StartAngle;

	/** 現在の角度 */
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	/** ノックバックの強さ */
	UPROPERTY(EditAnywhere)
	float KnockbackPower;

	UPROPERTY(EditAnywhere)
	bool bIsDrawDebugLine;
};