
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
//#include "UObject/Interface.h"

#include "GearSpecialActionState.generated.h"

class APlayerBase;
class UGearBase;

/**
 * 
 */

UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UGearSpecialAction : public UObject
{
	GENERATED_BODY()
public:	
	UGearSpecialAction();
public:

	/** Getter */
	inline float GetCameraRestoreElapsedTime()	const { return CameraRestoreElapsedTime; }

	/**
	 * @brief カメラステータスを保存
	 * ※ギアスキル発動前に呼び、保存する
	 */
	void SaveCameraStatus(APlayerBase* Player);

	/**
	 * @brief カメラ復帰補間を更新
	 * @param DeltaTime デルタタイム
	 */
	void UpdateCameraRestoreInterpolation(
		APlayerBase* Player,
		UGearBase* Owner,
		int32 LevelIndex,
		float CameraRestoreDuration,
		float DeltaTime);

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
		APlayerBase* Player,
		const FQuat& InCurrentQuaternion,
		float InLerpAlpha);

	/**
	 * @brief StartTargetArmLengthへ補間する
	 * @param InCurrentTargetArmLength 開始カメラとプレイヤーとの距離
	 * @param InLerpAlpha 補間値
	 */
	void InterpToStartTargetArmLength(
		APlayerBase* Player,
		float InCurrentTargetArmLength,
		float InLerpAlpha);

	/** カメラ復帰処理の経過時間 */
	float CameraRestoreElapsedTime;
};
