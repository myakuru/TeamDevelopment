#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include "DashGearState_Lv4.generated.h"

/** ロボットコントローラークラス */
class ARobotController;

/** 残像攻撃エフェクトクラス */
class UAfterImageAttackEffect;


/** ダッシュギア4レベルのカメラデータ */
USTRUCT(BlueprintType)
struct FDashGearState_Lv4_CameraData
{
	GENERATED_BODY()
public:

	FDashGearState_Lv4_CameraData() :
		Time(0.0f),
		TargetArmLength(0.0f),
		ArmLengthLerpSpeed(1.0f),
		TargetRotator(FRotator()),
		RotatorLerpSpeed(1.0f)
	{
	}

public:

	/** 区間時間 */
	UPROPERTY(EditAnywhere)
	float Time;

	/** カメラとプレイヤーの距離 */
	UPROPERTY(EditAnywhere)
	float TargetArmLength;

	/** カメラとプレイヤーの距離補間速度 */
	UPROPERTY(EditAnywhere)
	float ArmLengthLerpSpeed;

	/** 目指すべきカメラ回転 */
	UPROPERTY(EditAnywhere)
	FRotator TargetRotator;

	/** 回転補間速度 */
	UPROPERTY(EditAnywhere)
	float RotatorLerpSpeed;
};


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
		class UGearBase* Gear)				override;
	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

private:

	/**
	 * @brief 戦闘構え状態を更新
	 * @param ElapsedTime 
	 */
	void UpdateCombatStance(float ElapsedTime);

	void UpdateCamera(float DeltaTime);

	void UpdateCameraRotation(float DeltaTime, int32 DataIndex);

	void UpdateTargetArmLength(float DeltaTime, int32 DataIndex);


	int32 GetCurrentSectionIndex(float InElapsedTime);

	static const int32 kLv4Index = 3;

	/** ギアスキル開始時プレイヤーのTransform */
	FTransform StartPlayerTransform;
	FRotator StartControlRotation;

	/** ロボットコントローラークラス */
	UPROPERTY()
	TObjectPtr<ARobotController> RobotController;

	/** 残像攻撃エフェクトクラス */
	UPROPERTY(EditAnywhere,Instanced,Category = "Ghost")
	TObjectPtr<UAfterImageAttackEffect>  AfterImageAttackEffect;

	/** 構えアニメーションの時間閾値 */
	UPROPERTY(EditAnywhere)
	float StanceMinTimeThreshold;

	UPROPERTY(EditAnywhere)
	float StanceMaxTimeThreshold;

	/** カメラデータをまとめる配列 */
	UPROPERTY(EditAnywhere, Category = "Camera")
	TArray<FDashGearState_Lv4_CameraData> CameraData;

	UPROPERTY()
	float StartTargetArmLength;
};
