#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "FanAttackBase.generated.h"

class UEnemyManagerSubsystem;

/**
 * 扇状攻撃コンポーネントの基底クラス
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFanAttackBase : public UAttackBase
{
	GENERATED_BODY()
	
public:

	UFanAttackBase();

public:

	/**
	 * @brief 攻撃開始時の初期化
	 */
	virtual void Start();

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute()override;
	
	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 * @return 更新結果
	 */
	virtual void Update(float DeltaTime)override;

	/**
	 * @brief 攻撃判定の更新
	 * @param DeltaTime デルタタイム
	 * @return 攻撃が終了しているならtrue
	 */
	virtual bool UpdateAttack(float DeltaTime);

	/**
	 * @brief まだ生存していて削除可能か
	 * @return 削除可能ならtrue
	 */
	bool CanDeactivate();

	/**
	 * @brief 有効化された瞬間のフレームか
	 * @return 有効化されたフレームならtrue
	 */
	bool IsActiveFirstFrame();

	/**
	 * @brief PrevFlagを更新
	 */
	void UpdatePrevActiveFlg();

	/**
	 * @brief 攻撃範囲内にターゲットが居るかの判定
	 * @param Target ターゲット
	 * @return 攻撃範囲内ならtrue
	 */
	virtual bool IsTargetInRange(AActor* Target) override;

	/// <summary>
	/// 攻撃方向の計算
	/// </summary>
	/// <param name="forwardVector">前方方向</param>
	/// <returns>計算結果</returns>
	
	/**
	 * @brief 攻撃方向を算出
	 * @param forwardVector 前方ベクトル
	 * @return 計算後のベクトル
	 */
	virtual FVector CalcAttackDir(const FVector& forwardVector) const override;
	FVector CalcAttackDir(const FVector& forwardVector,float Angle) const;

	/**
	 * @brief 半径の二乗を取得
	 * @return 二乗した結果
	 */
	inline float GetRadiusSquared() const { return Radius * Radius; }

	/**
	 * @brief 扇角のcos値を取得
	 * @return cos値に変換した結果
	 */
	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

protected:

	/**
	 * @brief 攻撃判定を、「対敵リスト」「対プレイヤー」かを判断
	 */
	virtual void AttackJudge()override;

	/**
	 * @brief 敵リストに対する攻撃判定
	 * @param EnemyManager 敵管理クラスのアドレス
	 */
	virtual void AttackJudgeEnemys(const TObjectPtr<UEnemyManagerSubsystem>& a_EnemyManager);

	/**
	 * @brief プレイヤーに対する攻撃判定
	 * @param Player プレイヤークラスのアドレス
	 */
	virtual void AttackJudgePlayer(const TObjectPtr<AActor>& a_Player);

public:

	// 攻撃の持続時間（秒）
	UPROPERTY(EditAnywhere)
	float Duration;

	// 経過時間
	float ElapsedTime;

	// 回転するかどうか
	UPROPERTY(EditAnywhere)
	bool bRotate;

	// 回転速度（度/秒）
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	// 攻撃半径
	UPROPERTY(EditAnywhere)
	float Radius;

	// 扇の広さ（角度）
	UPROPERTY(EditAnywhere)
	float ConeAngle;

	// 全フレームでアクティブだったか
	bool bPrevActive;

	// 開始の角度
	UPROPERTY(EditAnywhere)
	float StartAngle;

	// 現在の角度
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	// ノックバックの強さ
	UPROPERTY(EditAnywhere)
	float KnockbackPower;

	UPROPERTY(EditAnywhere)
	bool bIsDrawDebugLine;
};