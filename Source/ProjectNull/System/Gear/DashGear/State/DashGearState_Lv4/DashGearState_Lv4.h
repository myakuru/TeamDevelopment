#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>
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
	void Initialize(class APlayerBase* Player,
					class UPlayerGearComponent* GearComponent,
					class UGearBase* Gear)	override;
	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	/** ギアレベル4の配列インデックス */
	static const int32 kLv4Index = 3;

private:

	/**
	 * @brief ギア発動時間初期化
	 */
	void InitializeGearDuration();

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
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 
	 * @param DataIndex 更新したいデータインデックス
	 */
	void UpdateCameraRotation(float DeltaTime, float InElapsedTime, int32 DataIndex);

	/**
	 * @brief プレイヤーとカメラとの距離更新
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 * @param DataIndex 更新したいデータインデックス
	 */
	void UpdateTargetArmLength(float DeltaTime, float InElapsedTime, int32 DataIndex);


	/**
	 * @brief 最終ダッシュの更新処理
	 * @param DeltaTime デルタタイム
	 * @param ElapsedTime 経過時間
	 */
	void UpdateFinalDash(float DeltaTime, float ElapsedTime);

	
	/**
	 * @brief 経過時間に基づいて、どの区間かどうか調べ、インデックスを返す
	 * @param InElapsedTime 経過時間
	 * @return 区間インデックス
	 */
	int32 GetCurrentSectionIndex(float InElapsedTime);

	/**
	 * @brief その地点までの経過時間
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

	/** ギアスキル開始時カメラ回転 */
	FRotator StartControlRotation;

	/** ギアスキル開始時カメラ距離 */
	UPROPERTY()
	float StartTargetArmLength;

};
