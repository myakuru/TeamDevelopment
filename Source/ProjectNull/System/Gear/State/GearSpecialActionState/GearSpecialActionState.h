
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include "GearSpecialActionState.generated.h"

class APlayerBase;
class UGearBase;
class UGearStateBase;
class ARobotController;

/**
 * 
 */

UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UGearSpecialAction : public UObject
{
	GENERATED_BODY()
public:	
	UGearSpecialAction();

	void Initialize(
		class UGearStateBase* InOwner);

	void Update(
		float DeltaTime,
		float InElapsedTime);

	void Execute(const FTransform& StartTransform);

	/** Getter */
	inline float	GetCameraRestoreElapsedTime()	const { return CameraRestoreElapsedTime; }
	inline float	GetCameraRestoreDuration()		const { return CameraRestoreDuration; }
	float			GetTotalDuration()				const;
private:

	/**
	 * @brief カメラ復帰補間を更新
	 * @param DeltaTime デルタタイム
	 */
	void UpdateCameraRestoreInterpolation(float DeltaTime);

	/** ギアスキル開始時カメラ回転 */
	FRotator StartControlRotation;

	/** カメラ復帰開始時カメラ回転 */
	FRotator RestoreStartControlRotation;

	/** ギアスキル開始時カメラ距離 */
	float StartTargetArmLength;

	/** カメラ復帰開始時カメラ距離 */
	float RestoreStartTargetArmLength;



	/**
	 * @brief カメラステータスを保存
	 * ※ギアスキル発動前に呼び、保存する
	 */
	void SaveCameraStatus();

	/**
	 * @brief 復元する際の開始データ初期化
	 */
	void InitializeRestoreStartData();

	/**
	 * @brief カメラの回転更新処理
	 * @param DataIndex 更新したいデータインデックス
	 * @param InLerpAlpha 補間値
	 */
	void UpdateCameraRotation(
		int32 DataIndex,
		float InLerpAlpha);

	/**
	 * @brief プレイヤーとカメラとの距離更新
	 * @param DataIndex 更新したいデータインデックス
	 * @param InLerpAlpha 補間値
	 */
	void UpdateTargetArmLength(
		int32 DataIndex,
		float InLerpAlpha);

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

	/**
	 * @brief 前区間の有効なカメラデータ取得
	 * @param DataIndex データインデックス
	 * @return カメラデータ
	 */
	const FCameraSequenceData* GetPreviousValidCameraData(int32 DataIndex) const;

	/**
	 * @brief 経過時間に基づいて、どの区間かどうか調べ、インデックスを返す
	 * @param InElapsedTime 経過時間
	 * @return 区間インデックス
	 */
	int32 GetCurrentSectionIndex(float InElapsedTime);

	/**
	 * @brief 区間内での開始時間
	 * @param InTargetIndex 計算する地点インデックス
	 * @return 経過時間
	 */
	float GetElapsedTimeToIndex(int32 InTargetIndex);


	UPROPERTY()
	TObjectPtr<APlayerBase> Player;

	UPROPERTY()
	TObjectPtr<UGearBase> Gear;

	UPROPERTY()
	TObjectPtr<UGearStateBase> Owner;

	/** ロボットコントローラークラス */
	UPROPERTY()
	TObjectPtr<ARobotController>			RobotController;

	/** カメラ復帰処理の経過時間 */
	float CameraRestoreElapsedTime;

	/** カメラを元に戻すまでの時間 */
	UPROPERTY(EditAnywhere)
	float CameraRestoreDuration;

	/** カメラデータをまとめる配列 */
	UPROPERTY(EditAnywhere)
	TArray<FCameraSequenceData> CameraSequenceData;

	/** ギアスキル開始時プレイヤーのTransform */
	FTransform StartPlayerTransform;
};
