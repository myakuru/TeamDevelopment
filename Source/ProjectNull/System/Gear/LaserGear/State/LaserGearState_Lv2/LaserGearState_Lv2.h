
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>

#include "LaserGearState_Lv2.generated.h"

/** レーザーギアの2レベル状態クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv2 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv2();
public:

	void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner)			override;

	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
	void End()								override;
	inline const int32 GetGearLevelIndex() const	override { return kLv2Index; }

private:

	/** 反射レーザー(弾タイプ)の発射制御を行うクラス */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<ULaserBulletShooter> ReflectiveLaserBulletShooter;
};
