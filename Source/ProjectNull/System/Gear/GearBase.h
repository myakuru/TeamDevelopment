#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearBase.generated.h"

/** ギアステータス */
USTRUCT(BlueprintType)
struct FGearStatus
{
	GENERATED_BODY()
public:

	FGearStatus():
		Duration(0.f),
		CoolTime(0.f),
		SimultaneousActivationCoolTime(0.f)
	{
	}

public:

	/** ギア発動時間 */
	UPROPERTY(EditAnywhere)
	float Duration;

	/* ギアクールタイム */
	UPROPERTY(EditAnywhere)
	float CoolTime;

	/** ギアの同時発動を許可するまでのクールタイム */
	UPROPERTY(EditAnywhere)
	float SimultaneousActivationCoolTime;
};

/** プレイヤーギアコンポーネントクラス */
class UPlayerGearComponent;

/** プレイヤー中間基底クラス */
class APlayerBase;

/** ギアの状態基底クラス */
class UGearStateBase;


/** ギアの基底クラス */
UCLASS()
class PROJECTNULL_API UGearBase : public UObject
{
	GENERATED_BODY()
public:
	UGearBase();
public:

	/** 最大ギアレベル */
	static constexpr int32 kMaxGearLevel = 4;

	/**
	 * @brief ギアの初期化処理
	 * @param Player プレイヤークラス
	 * @param GearComponent ギアコンポーネントクラス
	 */
	virtual void Initialize(class APlayerBase* Player,
							class UPlayerGearComponent* GearComponent);

	/**
	 * @brief ギアの実行処理
	 (実行の1フレームだけ呼ぶ)
	 * @param CurrentGearLevel 現在のギアレベル
	 */
	virtual void Execute(int32 CurrentGearLevel);

	/**
	 * @brief ギアの更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float DeltaTime);

	void ForceStop();

	/** Getter */
	inline float GetElapsedTime()		const		{ return ElapsedTime; }
	inline bool CanExecute()			const		{ return bCanExecute; }
	inline bool BlocksMovement()		const		{ return bBlocksMovement; }
	inline bool IsActive()				const		{ return bIsActive; }
	inline bool IsMovementBlocked()		const		{ return bBlocksMovement; }
	inline FTimerHandle GetCoolTimerHandle() const	{ return CoolTimerHandle; }
	inline bool AllowOtherGearActivation()	const	{ return bAllowOtherGearActivation; }

	float GetGearDuration(int32 Index) const;
	float GetGearCoolTime(int32 Index) const;

	/** Setter */
	inline void SetBlocksMovement(bool bInBlocksMovement)	{ bBlocksMovement = bInBlocksMovement; }
	inline void SetCanExecute(bool bInCanExecute)			{ bCanExecute = bInCanExecute; }
	inline void SetGearIndex(int32 InGearIndex)				{ GearIndex = InGearIndex; }
	void		SetGearDuration(float InDuration, int32 Index);


protected:

	/** 持ち主のプレイヤークラス */
	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	/** 持ち主のギアコンポーネントクラス */
	UPROPERTY()
	TObjectPtr<UPlayerGearComponent> OwnerGearComponent;

private:

	/**
	 * @brief ギアの発動時間終了時のリセット処理
	 */
	virtual void Reset();

	/** ギアの状態配列 */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UGearStateBase>> GearStates;

	/** ギアのステータス配列 */
	UPROPERTY(EditAnywhere)
	TArray<FGearStatus> GearStatuses;

	/** 現在のギア状態 */
	TObjectPtr<UGearStateBase> CurrentGearState;

	/** 実行時のギアレベルを保持する用途 */
	int32 ExecutedGearLevel;

	/** ギアスキル実行が可能かどうか */
	bool bCanExecute;

	/** ギアスキル実行開始からの経過時間 */
	float ElapsedTime;

	/** ギアスキル発動時間 */
	float Duration;

	/** ギアの同時発動を許可するまでのクールタイム */
	float SimultaneousActivationCoolTime;

	/** ギアスキルが実行されているかどうか */
	bool bIsActive;

	/** ギアによって移動が阻害されているかどうか */
	bool bBlocksMovement;

	/** ギアの発動時間管理用タイマーハンドル */
	FTimerHandle CoolTimerHandle;

	/** ギアの装備インデックス */
	int32 GearIndex;

	/** このギアの動作中に、他のギアの発動を許可するか */
	bool bAllowOtherGearActivation;
};
