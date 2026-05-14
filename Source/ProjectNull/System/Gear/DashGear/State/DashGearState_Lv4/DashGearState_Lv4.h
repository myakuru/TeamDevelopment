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
		TargetLength(0.0f),
		TargetRotator(FRotator()),
		LerpSpeed(1.0f)
	{
	}

public:

	/** 区間時間 */
	UPROPERTY(EditAnywhere)
	float Time;

	/** カメラとプレイヤーの距離 */
	UPROPERTY(EditAnywhere)
	float TargetLength;

	/** 目指すべきカメラ回転 */
	UPROPERTY(EditAnywhere)
	FRotator TargetRotator;

	/** 補間速度 */
	UPROPERTY(EditAnywhere)
	float LerpSpeed;
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
	
private:

	/**
	 * @brief 戦闘構え状態を更新
	 * @param ElapsedTime 
	 */
	void UpdateCombatStance(float ElapsedTime);

	void UpdateCamera(float DeltaTime);

	void SetMeshVisibility(bool bInVisibility) const;
	void SetMeshHiddenInGame(bool bInHiddenInGame) const;

	static const int32 kLv4Index = 3;

	/** ギアスキル開始時プレイヤーのTransform */
	FTransform StartPlayerTransform;

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
	UPROPERTY(EditAnywhere)
	TArray<FDashGearState_Lv4_CameraData> CameraData;
};
