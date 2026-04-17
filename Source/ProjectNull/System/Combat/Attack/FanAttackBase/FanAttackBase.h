#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "FanAttackBase.generated.h"

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

	/// <summary>
	/// 開始の際の初期化
	/// </summary>
	void Start();
	 
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	/// /// <returns>更新結果</returns>
	bool UpdateAttack(float DeltaTime);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CanDeactivate();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsActiveFirstFrame();

	/// <summary>
	/// 
	/// </summary>
	void UpdatePrevActiveFlg();

	/// <summary>
	/// 攻撃範囲内にターゲットがいるかどうかの判定
	/// </summary>
	/// <param name="Target			">ターゲット	</param>
	/// <param name="OwnerLocation	">攻撃者の位置	</param>
	/// <returns>ターゲットが攻撃範囲内かどうか</returns>
	virtual bool IsTargetInRange(AActor* Target, const FVector& OwnerLocation) override;

	/// <summary>
	/// 攻撃方向の計算
	/// </summary>
	/// <param name="forwardVector">前方方向</param>
	/// <returns>計算結果</returns>
	virtual FVector CalcAttackDir(const FVector& forwardVector) const override;

	/// <summary>
	/// 半径の二乗を取得
	/// </summary>
	/// <returns>半径の二乗</returns>
	inline float GetRadiusSquared() const { return Radius * Radius; }

	/// <summary>
	/// 扇角のcos値
	/// </summary>
	/// <returns>扇角のcos値</returns>
	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

protected:

	/// <summary>
	/// 敵リストに対する攻撃判定
	/// </summary>
	/// <param name="EnemyManager">敵管理クラスのアドレス</param>
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) override;

	/// <summary>
	/// プレイヤーに対する攻撃判定
	/// </summary>
	/// <param name="Player">プレイヤークラスのアドレス</param>
	virtual void AttackJudgePlayer(APlayerBase* Player) override;

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

	// 攻撃中か
	bool bIsActive;

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

};