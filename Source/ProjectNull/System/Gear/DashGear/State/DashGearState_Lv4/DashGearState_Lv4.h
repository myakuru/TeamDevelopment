#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/DashGear/State/DashGearStateBase.h>

#include "DashGearState_Lv4.generated.h"

class UGearSpecialAction;

/** ロボットコントローラークラス */
class ARobotController;

/** 残像攻撃エフェクトクラス */
class UAfterImageAttackEffect;

/** アニメーションモンタージュ */
class UAnimMontage;

/** ダッシュギアのレベル4状態クラス */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv4 final : public UDashGearStateBase
{
	GENERATED_BODY()
public:
	UDashGearState_Lv4();
public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	inline const int32 GetGearLevelIndex() const	override { return kLv4Index; }

private:

	/**
	 * @brief ギア発動時間初期化
	 */
	void InitializeGearDuration();

	/**
	 * @brief 戦闘構え状態を更新
	 * @param ElapsedTime 経過時間
	 */
	void UpdateCombatStance(float ElapsedTime);

	/**
	 * @brief 攻撃用スフィア判定更新
	 * @param ElapsedTime 経過時間
	 */
	void UpdateAttackSphereCollision(float ElapsedTime);

	/**
	 * @brief 最終ダッシュの更新処理
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 */
	void UpdateFinalDash(
		float DeltaTime,
		float ElapsedTime);

	/**
	 * @brief 構え状態のアニメーション再生
	 */
	void PlayStanceAnimation();

	/**
	 * @brief 構え状態のアニメーションブレンドアウト
	 */
	void BlendOutStanceAnimation();

	/** ロボットコントローラークラス */
	UPROPERTY()
	TObjectPtr<ARobotController>			RobotController;

	/** 残像攻撃エフェクトクラス */
	UPROPERTY(EditAnywhere,Instanced,Category = "Ghost")
	TObjectPtr<UAfterImageAttackEffect>		AfterImageAttackEffect;

	/** 構えアニメーションの時間閾値 */
	UPROPERTY(EditAnywhere)
	FThresholdRange StanceTime;

	/** ダッシュの時間閾値 */
	UPROPERTY(EditAnywhere)
	FThresholdRange DashTime;

	/** 構え状態のアニメーションモンタージュ */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> StanceAnimMontage;

	UPROPERTY(EditAnywhere, Category = "Animation")
	float StanceAnimBlendOutTime;

	bool bExecuteFinalDash;

	UPROPERTY(EditAnywhere)
	float SpecialAttackSphereRadius;

	UPROPERTY(EditAnywhere,Instanced)
	TObjectPtr<UGearSpecialAction> GearSpecialAction;
};
