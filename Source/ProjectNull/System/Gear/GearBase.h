#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearBase.generated.h"

//　プレイヤーのギアコンポーネントクラス
class UPlayerGearComponent;

//　プレイヤーの中間基底クラス
class APlayerBase;


UCLASS()
class PROJECTNULL_API UGearBase : public UObject
{
	GENERATED_BODY()
	
public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(class APlayerBase* Player,class UPlayerGearComponent* GearComponent);

	/// <summary>
	/// ギアの実行
	/// ※純粋仮想関数であり、派生クラスで必ず実装する必要がある
	/// </summary>
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(float DeltaTime);

	//　ゲッター
	bool CanExecute() const { return bCanExecute; }
	
protected:
	
	/// <summary>
	/// 持ち主のプレイヤーのポインタ
	/// </summary>
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	//　持ち主ギアコンポーネントのポインタ
	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

	//　ギアを実行できるかどうか
	bool bCanExecute;

	//　ギアの効果が終了したかどうか
	bool bIsFinished;

	//　ギア持続時間
	UPROPERTY(EditAnywhere)
	float Duration;

	//　ギアの持続時間を管理するタイマーハンドル
	FTimerHandle DurationTimerHandle;

private:

	//　ギアのレベルに応じた効果の適用
	//　4段階のためメソッドを4つ用意するだけの簡単で分かりすい実装
	//　※純粋仮想関数であり、派生クラスで必ず実装する必要がある
	virtual void ApplyLv1() PURE_VIRTUAL(UAttackBase::Execute, );
	virtual void ApplyLv2() PURE_VIRTUAL(UAttackBase::Execute, );
	virtual void ApplyLv3() PURE_VIRTUAL(UAttackBase::Execute, );
	virtual void ApplyLv4() PURE_VIRTUAL(UAttackBase::Execute, );

	virtual void ResetParams() {}

};
