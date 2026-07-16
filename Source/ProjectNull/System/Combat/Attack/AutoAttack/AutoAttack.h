#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "AutoAttack.generated.h"

/** 敵管理クラス */
class UEnemyManagerSubsystem;

/** 円型の斬撃攻撃クラス */
class UFloatingWeaponAttack;

/** Niagaraエフェクトクラス */
class UFloatingWeaponEffect;

/** 自動攻撃種類 */
UENUM(BlueprintType)
enum class EAutoAttackType : uint8
{
	Front,
	Ring,
	Count UMETA(Hidden)
};


/** 自動攻撃クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UAutoAttack final : public UAttackBase
{
	GENERATED_BODY()
public:
	UAutoAttack();

	void Initialize(const TObjectPtr<AActor>& Owner)	override;
	void Update(float DeltaTime)						override;
	void Execute(const FVector& InTargetLocation = FVector::ZeroVector)		override { return; }

	void SetVisibility(bool bVisibility);
	
	/** Getter */
	inline float GetAutoAttackInterval() const	{ return AutoAttackInterval; }
	inline float GetFrontToRingDelay() const	{ return FrontToRingDelay; }

private:
	
	/**
	 * @brief 自動攻撃の開始
	 */
	void StartAutoAttack();

	/**
	 * @brief 周囲2段階（リング状に移動）自動斬撃攻撃の開始
	 */
	void StartAutoRingAttack();
	
	/** 自動攻撃の間隔時間 */
	UPROPERTY(EditAnywhere)
	float AutoAttackInterval;

	/** 前方扇状自動攻撃からの周囲攻撃遅延時間 */
	UPROPERTY(EditAnywhere)
	float FrontToRingDelay;

	/** 自動攻撃のパラメータマップ配列 */
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, TObjectPtr<UFloatingWeaponAttack>> AutoAttackParamsMap;

	/** 前方扇状自動攻撃タイマー*/
	FTimerHandle AutoFrontConeAttackTimerHandle;

	/** 前方扇状自動攻撃からftの周囲攻撃遅延タイマー */
	FTimerHandle FrontToRingDelayTimerHandle;
};
