
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponEffect.generated.h"

//　
//UENUM(BlueprintType)
//enum class EFloatingWeaponState : uint8
//{
//	Front,
//	Ring,
//	Count UMETA(Hidden)
//};

//　Niagaraエフェクトクラス
class UNiagaraSystem;

//　Niagaraコンポーネントクラス
class UNiagaraComponent;

//　リング状斬撃攻撃クラス
class URingPulseSlashAttack;

//　浮遊武器の状態基底クラス
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
	/// エフェクト再生開始
	/// </summary>
	void Start(USceneComponent* RootComponent);

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="OwnerActor">持ち主のクラス</param>
	void Update(AActor* OwnerActor);

	//　セッター
	inline void SetOwnerAttack(URingPulseSlashAttack* Owner) { OwnerAttack = Owner; }

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



	//　基準とする攻撃クラス
	UPROPERTY()
	URingPulseSlashAttack* OwnerAttack;

	//　Niagaraシステム
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* EffectSystem;

	//　Niagaraコンポーネント
	UPROPERTY()
	UNiagaraComponent* EffectComponent;

	//　エフェクトのTransform
	UPROPERTY(EditAnywhere)
	FTransform Transform;

	//　エフェクトの半径オフセット
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	//　エフェクトの回転オフセット
	UPROPERTY(EditAnywhere)
	FRotator RotatorOffset;

	//　浮遊武器の状態の配列
	//UPROPERTY(EditAnywhere, Instanced)
	//TMap< UFloatingWeaponStateBase*> FloatingWeaponStates;

	//　現在の浮遊武器状態
	UPROPERTY()
	UFloatingWeaponStateBase* CurrentFloatingWeaponState;
};
