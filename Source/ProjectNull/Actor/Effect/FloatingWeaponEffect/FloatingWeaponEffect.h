
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponEffect.generated.h"


/// <summary>
/// 浮遊武器状態種類 
/// </summary>
UENUM(BlueprintType)
enum class EFloatingWeaponState : uint8
{
	Stand,
	Attack,
	Transition,
	Count UMETA(Hidden)
};

// Niagaraエフェクトクラス
class UNiagaraSystem;

// Niagaraコンポーネントクラス
class UNiagaraComponent;

// 扇状斬撃攻撃クラス
class UFloatingWeaponAttack;

// 浮遊武器状態の中間基底クラス
class UFloatingWeaponStateBase;


/// <summary>
/// 浮遊武器エフェクトクラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponEffect : public UObject
{
	GENERATED_BODY()

public:
	UFloatingWeaponEffect();
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// エフェクト再生開始
	/// </summary>
	void Start(USceneComponent* RootComponent);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="OwnerActor">持ち主のクラス</param>
	void Update(float DeltaTime);

	/// <summary>
	/// 状態の遷移
	/// </summary>
	/// <param name="State">ステート種類</param>
	void ChangeState(EFloatingWeaponState NextState);
	void ChangeState(EFloatingWeaponState NextState,EFloatingWeaponState TheStateAfterTheNext);
	

	bool IsAttackStateStep() const;

	FTransform GetAttackStartTransformOffset();
	FTransform GetStandStartTransformOffset();

	// セッター
	inline void SetOwnerAttack(UFloatingWeaponAttack* Owner)		{ OwnerAttack = Owner; }
	inline void SetOwnerActor(AActor* SetOwnerActor)				{ OwnerActor = SetOwnerActor; }
	inline void SetRelativeTransform(const FTransform& a_RelativeTransform)	{ RelativeTransform = a_RelativeTransform; }
	
	// ゲッター
	inline UFloatingWeaponAttack* GetOwnerAttack() const	{ return OwnerAttack; }
	inline FTransform GetRelativeTransform() const			{ return RelativeTransform; }

private:

	/// <summary>
	/// Transformの更新
	/// </summary>
	void UpdateTransform();


	/// <summary>
	/// エフェクト無効にする
	/// </summary>
	void Deactivate();

	/// <summary>
	/// 出現可能かどうか
	/// </summary>
	/// <returns>可能ならtrue 可能じゃないならfalse</returns>
	inline bool CanSpawn() const { return EffectSystem && !EffectComponent; }

	// 基準とする攻撃クラス
	UPROPERTY()
	UFloatingWeaponAttack* OwnerAttack;

	// 持ち主のクラス
	UPROPERTY()
	AActor* OwnerActor;

	// Niagaraシステム
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* EffectSystem;

	// Niagaraコンポーネント
	UPROPERTY()
	UNiagaraComponent* EffectComponent;

	// エフェクトのRelativeTransform
	UPROPERTY()
	FTransform RelativeTransform;

	// 浮遊武器の状態の配列
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EFloatingWeaponState,UFloatingWeaponStateBase*> States;

	// 現在の浮遊武器状態
	UPROPERTY()
	UFloatingWeaponStateBase* CurrentState;
};
