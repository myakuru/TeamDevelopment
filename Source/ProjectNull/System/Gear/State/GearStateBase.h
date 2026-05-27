
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include "GearStateBase.generated.h"

/** プレイヤーの中間基底クラス */
class APlayerBase;

/** プレイヤーギアコンポーネントクラス */
class UPlayerGearComponent;

/** ギアの中間基底クラス */
class UGearBase;


/** ギアクラスの状態中間基底クラス
	1Lv,2Lv...(状態) */
UCLASS()
class PROJECTNULL_API UGearStateBase : public UObject
{
	GENERATED_BODY()
	
public:
	UGearStateBase();
public:

	/**
	 * @brief 初期化処理
	 * ゲーム開始前に1フレーム呼ばれる
	 * @param InPlayer プレイヤークラスのポインタ
	 * @param InGearComponent ギアコンポーネントクラスのポインタ
	 * @param InOwner 持ち主のギアクラス
	 */
	virtual void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner);

	/**
	 * @brief ギアスキルの実行処理
	 * @param CurrentGearLevel 現在のギアレベル
	 */
	virtual void Execute(int32 CurrentGearLevel) { return; }

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float DeltaTime) { return; }

	/**
	 * @brief 終了処理
	 ギアスキル終了時に1フレーム呼ばれる
	 */
	virtual void End() { return; }

	/** Getter */
	inline float GetCameraRestoreDuration() const			{ return CameraRestoreDuration; }
	inline float GetCameraRestoreElapsedTime() const		{ return CameraRestoreElapsedTime; }
	inline virtual const int32 GetGearLevelIndex() const	{ return kLv1Index; }

	/** ギアレベルの配列インデックス */
	static const int32 kLv1Index = 0;
	static const int32 kLv2Index = 1;
	static const int32 kLv3Index = 2;
	static const int32 kLv4Index = 3;

protected:

	/**
	 * @brief カメラステータスを保存
	 * ※ギアスキル発動前に呼び、保存する
	 */
	void SaveCameraStatus();

	/**
	 * @brief カメラ復帰補間を更新
	 * @param DeltaTime デルタタイム
	 */
	void UpdateCameraRestoreInterpolation(float DeltaTime);

	
	/** プレイヤーの中間基底クラス */
	UPROPERTY()
	TObjectPtr<APlayerBase> Player;

	/** プレイヤーギアコンポーネントクラス */
	UPROPERTY()
	TObjectPtr<UPlayerGearComponent> GearComponent;

	/** 持ち主のギアクラス */
	UPROPERTY()
	TObjectPtr<UGearBase> Owner;

	/** ギアスキル開始時カメラ回転 */
	FRotator StartControlRotation;

	/** カメラ復帰開始時カメラ回転 */
	FRotator RestoreStartControlRotation;

	/** ギアスキル開始時カメラ距離 */
	float StartTargetArmLength;

	/** カメラ復帰開始時カメラ距離 */
	float RestoreStartTargetArmLength;

private:

	/**
	 * @brief StartControlRotationへ補間する 
	 * @param InCurrentQuaternion 開始回転
	 * @param InLerpAlpha 補間値
	 */
	void InterpToStartControlRotation(
		const FQuat& InCurrentQuaternion,
		float InLerpAlpha);

	/**
	 * @brief StartTargetArmLengthへ補間する 
	 * @param InCurrentTargetArmLength 開始カメラとプレイヤーとの距離
	 * @param InLerpAlpha 補間値
	 */
	void InterpToStartTargetArmLength(
		float InCurrentTargetArmLength,
		float InLerpAlpha);


	/** カメラを元に戻すまでの時間 */
	UPROPERTY(EditAnywhere)
	float CameraRestoreDuration;

	/** カメラ復帰処理の経過時間 */
	float CameraRestoreElapsedTime;
};
