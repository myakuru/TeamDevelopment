#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearBase.generated.h"


//　ギアのステータス構造体
USTRUCT(BlueprintType)
struct FGearStatus
{
	GENERATED_BODY()
public:

	FGearStatus():
		Duration(0.0f),
		CoolTime(0.0f)
	{
	
	}
public:

	//　ギア持続時間
	UPROPERTY(EditAnywhere)
	float Duration;

	//　ギアのクールタイム
	UPROPERTY(EditAnywhere)
	float CoolTime;
};


//　プレイヤーのギアコンポーネントクラス
class UPlayerGearComponent;

//　プレイヤーの中間基底クラス
class APlayerBase;

//　ギアの状態の基底クラス
class UGearStateBase;


/// <summary>
/// ギアの基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UGearBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearBase();

public:

	//　ギアの最大レベル
	static constexpr int32 kMaxGearLevel = 4;

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(class APlayerBase* Player,class UPlayerGearComponent* GearComponent);

	/// <summary>
	/// ギアの実行
	/// </summary>
	virtual void Execute(int32 CurrentGearLevel);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(float DeltaTime);

	//　ゲッター
	bool CanExecute()			const { return bCanExecute; }
	bool BlocksMovement()		const { return bBlocksMovement; }
	bool IsActive()				const { return bIsActive; }
	bool IsMovementBlocked()	const { return bBlocksMovement; }

	//　セッター
	void SetBlocksMovement(bool Flg)	{ bBlocksMovement = Flg; }
	void SetCanExecute(bool Flg)		{ bCanExecute = Flg; }

protected:

	// 持ち主のプレイヤーのポインタ
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	//　持ち主ギアコンポーネントのポインタ
	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

private:

	virtual void ResetParams();


	//　ギアの状態の配列
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearStateBase*> GearStates;

	//　ギアのステータスの配列
	UPROPERTY(EditAnywhere)
	TArray<FGearStatus> GearStatuses;

	//　実行されたギアのレベル
	int32 ExecutedGearLevel;

	//　ギアを実行できるかどうか
	bool bCanExecute;

	//　経過時間
	float ElapsedTime;

	//　ギアが実行中かどうか
	bool bIsActive;

	//　ギアによって移動をブロックするかどうか
	bool bBlocksMovement;

	//　ギアの持続時間を管理するタイマーハンドル
	FTimerHandle DurationTimerHandle;
};
