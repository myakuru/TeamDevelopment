
#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/LaserGear/State/LaserGearStateBase.h>

#include "LaserGearState_Lv4.generated.h"

/** アニメーションモンタージュ */
class UAnimMontage;

class UEffectBase;

class ALaserbeam;

class UGearSpecialAction;

/**
 * 
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API ULaserGearState_Lv4 final : public ULaserGearStateBase
{
	GENERATED_BODY()
public:
	ULaserGearState_Lv4();
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
};
