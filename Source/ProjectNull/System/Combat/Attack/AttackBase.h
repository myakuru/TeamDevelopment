#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

/// <summary>
/// 攻撃の基底クラス
/// プレイヤーの攻撃コンポーネントクラスで使用される攻撃クラスの基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(class AActor* Owner);

	/// <summary>
	/// 攻撃の実行
	/// ※純粋仮想関数であり、派生クラスで必ず実装する必要がある
	/// </summary>
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(float DeltaTime) { return; }

	//　ゲッター
	bool CanExecute() const { return bCanExecute; }

protected:

	/// <summary>
	/// 持ち主のアクターのポインタ
	/// </summary>
	UPROPERTY()
	AActor* OwnerActor;
	
	// 攻撃を実行できるかどうか
	bool bCanExecute;
};
