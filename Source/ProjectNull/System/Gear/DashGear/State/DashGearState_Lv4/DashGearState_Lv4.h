#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/DashGear/State/DashGearStateBase.h>

#include "DashGearState_Lv4.generated.h"

/** ロボットコントローラークラス */
class ARobotController;

/** 残像攻撃エフェクトクラス */
class UAfterImageAttackEffect;


/** ダッシュギアのレベル4状態クラス */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv4 final : public UDashGearStateBase
{
	GENERATED_BODY()
public:
	UDashGearState_Lv4();
public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	inline const int32 GetGearLevelIndex() const	override { return kLv4Index; }

private:

	/**
	 * @brief ギア発動時間初期化
	 */
	void InitializeGearDuration();

	/**
	 * @brief 復元する際の開始データ初期化
	 */
	void InitializeRestoreStartData();

	/**
	 * @brief 戦闘構え状態を更新
	 * @param ElapsedTime 経過時間
	 */
	void UpdateCombatStance(float ElapsedTime);

	/**
	 * @brief カメラデータ更新処理
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 */
	void UpdateCameraData(float DeltaTime, float InElapsedTime);

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
	 * @brief 最終ダッシュの更新処理
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 */
	void UpdateFinalDash(
		float DeltaTime,
		float ElapsedTime);

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
	
	/** ロボットコントローラークラス */
	UPROPERTY()
	TObjectPtr<ARobotController> RobotController;

	/** 残像攻撃エフェクトクラス */
	UPROPERTY(EditAnywhere,Instanced,Category = "Ghost")
	TObjectPtr<UAfterImageAttackEffect>  AfterImageAttackEffect;

	/** 構えアニメーションの時間閾値 */
	UPROPERTY(EditAnywhere)
	FThresholdRange StanceTime;

	/** ダッシュの時間閾値 */
	UPROPERTY(EditAnywhere)
	FThresholdRange DashTime;

	/** カメラデータをまとめる配列 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	TArray<FCameraSequenceData> CameraData;

	/** ギアスキル開始時プレイヤーのTransform */
	FTransform StartPlayerTransform;

};
