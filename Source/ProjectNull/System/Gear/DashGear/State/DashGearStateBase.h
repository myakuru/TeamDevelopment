#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "DashGearStateBase.generated.h"

//　Niagaraエフェクトクラス
class UNiagaraSystem;


/// <summary>
/// ダッシュギアの状態の中間基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UDashGearStateBase : public UGearStateBase
{
	GENERATED_BODY()

public:	

	UDashGearStateBase();

public:

	virtual void Execute(int32 CurrentGearLevel) override;
	virtual void Update(float DeltaTime) override;

protected:
	
	/// <summary>
	/// ダッシュ処理
	/// </summary>
	void Dash();

	/// <summary>
	/// ダッシュ攻撃エフェクトの再生
	/// </summary>
	void PlayDashEffect();

	//　扇状（前方）自動斬撃攻撃のエフェクト
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DashEffect;

private:

	/// <summary>
	/// ダッシュ攻撃の更新
	/// </summary>
	void UpdateDashAttack();

	//　ダッシュ攻撃の距離の二乗（比較用）
	UPROPERTY(EditAnywhere)
	float DashAttackRangeSquared;

	//　ダッシュの速度
	UPROPERTY(EditAnywhere)
	float DashSpeed;

	//　ダッシュエフェクトの持続時間
	UPROPERTY(EditAnywhere)
	float DashEffectDuration;
};
