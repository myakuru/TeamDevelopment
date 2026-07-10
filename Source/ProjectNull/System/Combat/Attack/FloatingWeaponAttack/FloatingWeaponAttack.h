
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Combat/Attack/AttackBase.h>

#include "FloatingWeaponAttack.generated.h"


class AAutoAttackHitActor;

/** 浮遊武器のエフェクト */
class UFloatingWeaponEffect;

/** 斬撃のエフェクト */
class USlashEffectBase;

/** 自動攻撃クラス */
class UAutoAttack;

/** 浮遊武器の攻撃クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttack final : public UAttackBase
{
	GENERATED_BODY()
public:

	UFloatingWeaponAttack();

public:

	void Initialize(const TObjectPtr<AActor>& Owner)			override;
	void Update(float DeltaTime)								override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute()override;


	bool IsAttackStateStep();

	float TotalTransitionStateTime();

	float StandStateTime();

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

	FVector CalcAttackDir(
		const FVector& ForwardVector,
		float Angle) const;


	/** セッター */
	inline void SetAutoAttack(UAutoAttack* Attack) { AutoAttack = Attack; }

	/** ゲッター */
	inline UAutoAttack* GetAutoAttack() const { return AutoAttack; }
	inline AAutoAttackHitActor* GetAutoAttackHitActor() const { return AutoAttackHitActor; }
	inline float GetStartAngle() const { return StartAngle; }
	inline float GetCurrentAngle() const { return CurrentAngle; }
	
private:

	void UpdateRotation(float DeltaTime);

	void AlignFloor(float DeltaTime);

	/** 自動攻撃のポインタ */
	UPROPERTY()
	TObjectPtr<UAutoAttack> AutoAttack;

	/** 浮遊武器エフェクトクラス */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect")
	TObjectPtr<UFloatingWeaponEffect> FloatingWeaponEffect;

	/** 斬撃エフェクト */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect")
	TArray<TObjectPtr<USlashEffectBase>> SlashEffectArray;


	UPROPERTY()
	TObjectPtr<AAutoAttackHitActor> AutoAttackHitActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AAutoAttackHitActor> AutoAttackHitActorClass;

	/** 待機状態の割合（0.0～1.0）*/
	UPROPERTY(EditAnywhere)
	float StandTimeRatio;


	// 攻撃の持続時間（秒）
	UPROPERTY(EditAnywhere)
	float Duration;

	// 経過時間
	float ElapsedTime;

	// 回転速度（度/秒）
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

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
	float RotationInterpSpeed;
};
