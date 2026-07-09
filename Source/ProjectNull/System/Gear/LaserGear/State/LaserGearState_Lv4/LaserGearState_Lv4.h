
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>
#include <ProjectNull/Utility/Common/GameTypes/GameTypes.h>

#include "LaserGearState_Lv4.generated.h"

/** アニメーションモンタージュ */
class UAnimMontage;

class UEffectBase;

class ALaserbeam;

class UGearSpecialAction;

class ARobotController;

class UPlayerRuntimeData;

USTRUCT()
struct FRotationYaw
{
	GENERATED_BODY()
public:
	FRotationYaw():
		Time(0.f),
		TargetYawOffset(0.f)
	{
	}
	
	UPROPERTY(EditAnywhere)
	float Time;	
	
	UPROPERTY(EditAnywhere)
	float TargetYawOffset;
	
};

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv4 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv4();
	
	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;

	inline const int32 GetGearLevelIndex() const	override { return kLv4Index; }

private:
	
	void UpdateRotation(
		float InDeltaTime,
		float InElapsedTime);
	
	int32 GetCurrentSectionIndex(float InElapsedTime);

	
	float GetElapsedTimeToIndex(int32 InTargetIndex);

	/**
	* @brief 前区間の有効なカメラデータ取得
	* @param DataIndex データインデックス
	* @return カメラデータ
	*/
	const FRotationYaw* GetPreviousValidRotationYawData(int32 DataIndex) const;

	
	/** ロボットコントローラークラス */
	UPROPERTY()
	TObjectPtr<ARobotController> RobotController;
	
	/** Spell状態のアニメーションモンタージュ */
	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> SpellAnimMontage;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<ALaserbeam> Laserbeam;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ALaserbeam> LaserbeamClass;

	UPROPERTY(EditAnywhere)
	float SpellAnimBlendOutTime;

	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UGearSpecialAction> GearSpecialAction;

	UPROPERTY(EditAnywhere)
	TArray<FRotationYaw> RotationYaws;
	
	FTransform StartTransform;
	
	bool bSpawnEnable;
	
	UPROPERTY(EditAnywhere)
	FThresholdRange SpawnLaserThresholdRange;
	
	UPROPERTY(EditAnywhere)
	float FlyingTime;
	
	UPROPERTY(EditAnywhere)
	float WalkingTime;
	
	UPROPERTY(EditAnywhere)
	float TargetLocationOffsetZ;
	
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;
};
