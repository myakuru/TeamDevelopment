#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "RingPulseSlashAttack.generated.h"


/// <summary>
/// 円型の斬撃攻撃クラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API URingPulseSlashAttack final : public UAttackBase
{
	GENERATED_BODY()
	
public:

	URingPulseSlashAttack();

public:

	// 攻撃の持続時間（秒）
	UPROPERTY(EditAnywhere)
	float Duration;

	// 経過時間
	float ElapsedTime;

	//　回転するかどうか
	UPROPERTY(EditAnywhere)
	bool bRotate;

	//　回転速度（度/秒）
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	//　攻撃半径
	UPROPERTY(EditAnywhere)
	float Radius;

	//　扇の広さ（角度）
	UPROPERTY(EditAnywhere)
	float ConeAngle;

	//　実行中かどうか
	bool bIsActive;

	//　現在の角度
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

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
	/// 攻撃方向の計算
	/// </summary>
	/// <param name="forwardVector">前方方向</param>
	/// <returns>計算結果</returns>
	FVector CalcAttackDir(const FVector& forwardVector) const;

	/// <summary>
	/// 半径の二乗を取得
	/// </summary>
	/// <returns>半径の二乗</returns>
	inline float GetRadiusSquared() const { return Radius * Radius;}

	/// <summary>
	/// 扇角のcos値
	/// </summary>
	/// <returns>扇角のcos値</returns>
	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

};
