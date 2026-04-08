
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponEffect.generated.h"

//　Niagaraエフェクトクラス
class UNiagaraSystem;

//　Niagaraコンポーネントクラス
class UNiagaraComponent;

//　リング状斬撃攻撃クラス
class URingPulseSlashAttack;


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
};
