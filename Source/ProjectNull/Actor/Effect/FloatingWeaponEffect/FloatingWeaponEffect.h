
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponEffect.generated.h"

/** 浮遊武器状態種類 */
UENUM(BlueprintType)
enum class EFloatingWeaponState : uint8
{
	Stand,
	Attack,
	Transition,
	Count UMETA(Hidden)
};

/** Niagaraエフェクトクラス */
class UNiagaraSystem;

/** Niagaraコンポーネントクラス */
class UNiagaraComponent;

/** 扇状斬撃攻撃クラス */
class UFloatingWeaponAttack;

/** 浮遊武器状態の中間基底クラス */
class UFloatingWeaponStateBase;


/** 浮遊武器エフェクトクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponEffect : public UObject
{
	GENERATED_BODY()

public:
	UFloatingWeaponEffect();
public:
	
	/**
	 * @brief 初期化
	 */
	void Initialize();

	/**
	 * @brief エフェクト再生開始
	 * @param RootComponent ルートコンポーネント
	 */
	void Start(USceneComponent* RootComponent);

	/**
	 * @brief 更新
	 * @param DeltaTime デルタタイム
	 */
	void Update(float DeltaTime);

	/**
	 * @brief 状態の遷移
	 * @param NextState ステート種類
	 */
	void ChangeState(EFloatingWeaponState NextState);
	void ChangeState(EFloatingWeaponState NextState,EFloatingWeaponState TheStateAfterTheNext);
	

	bool IsAttackStateStep() const;

	FTransform GetAttackStartTransformOffset();
	FTransform GetStandStartTransformOffset();

	/** セッター */
	inline void SetOwnerAttack(UFloatingWeaponAttack* Owner)		{ OwnerAttack = Owner; }
	inline void SetOwnerActor(AActor* SetOwnerActor)				{ OwnerActor = SetOwnerActor; }
	inline void SetRelativeTransform(const FTransform& a_RelativeTransform)	{ RelativeTransform = a_RelativeTransform; }
	
	/** ゲッター */
	inline UFloatingWeaponAttack* GetOwnerAttack() const	{ return OwnerAttack; }
	inline FTransform GetRelativeTransform() const			{ return RelativeTransform; }

private:

	/**
	 * @brief Transformの更新
	 */
	void UpdateTransform();

	/**
	 * @brief エフェクト無効にする
	 */
	void Deactivate();

	/**
	 * @brief 出現可能かどうか
	 * @return 可能ならtrue 可能じゃないならfalse
	 */
	inline bool CanSpawn() const { return EffectSystem && !EffectComponent; }


	/** 基準とする攻撃クラス */
	UPROPERTY()
	TObjectPtr<UFloatingWeaponAttack> OwnerAttack;

	/** 持ち主のクラス */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	/** Niagaraシステム */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> EffectSystem;

	/** Niagaraコンポーネント */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> EffectComponent;

	/** エフェクトのRelativeTransform */
	UPROPERTY()
	FTransform RelativeTransform;

	/** 浮遊武器の状態の配列 */
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EFloatingWeaponState, TObjectPtr<UFloatingWeaponStateBase>> States;

	/** 現在の浮遊武器状態 */
	UPROPERTY()
	TObjectPtr<UFloatingWeaponStateBase> CurrentState;

};
