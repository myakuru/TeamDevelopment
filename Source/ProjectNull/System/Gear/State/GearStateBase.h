
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

protected:


	void SaveCameraStatus(APlayerBase* InPlayer);

	// StartControlRotationへ補間する 

	void InterpToStartControlRotation(const FQuat& InCurrentQuaternion,float InLerpAlpha);
	void InterpToStartTargetArmLength(float InCurrentTargetArmLength,float InLerpAlpha);
	

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

	/** ギアスキル開始時カメラ距離 */
	UPROPERTY()
	float StartTargetArmLength;

	/** カメラを元に戻すまでの時間 */
	UPROPERTY(EditAnywhere)
	float CameraRestoreDuration;
	
	/** カメラ復帰処理の経過時間 */
	UPROPERTY(EditAnywhere)
	float CameraRestoreElapsedTime;
	
};
