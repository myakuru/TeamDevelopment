
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearStateBase.generated.h"

// プレイヤーのギアコンポーネントクラス
class UPlayerGearComponent;

// プレイヤーの中間基底クラス
class APlayerBase;

// ギアの基底クラス
class UGearBase;

/// <summary>
/// ギアの状態の基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UGearStateBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearStateBase();

public:

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent,class UGearBase* Gear);

	/// <summary>
	/// ギアの実行
	/// </summary>
	virtual void Execute(int32 CurrentGearLevel) { return; }

	/// <summary>
	/// ギア状態の更新
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(float DeltaTime) { return; }

	// ギア状態の終了
	virtual void End() { return; }

protected:

	// 持ち主のプレイヤーのポインタ
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// 持ち主ギアコンポーネントのポインタ
	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

	// 持ち主ギアのポインタ
	UPROPERTY()
	UGearBase* OwnerGear;

};
