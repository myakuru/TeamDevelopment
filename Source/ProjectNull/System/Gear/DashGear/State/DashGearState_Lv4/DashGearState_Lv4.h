#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include "DashGearState_Lv4.generated.h"

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
		class UGearBase* Gear)				override;
	void Execute(int32 CurrentGearLevel)	override;
	void Update(float DeltaTime)			override;
private:

	static const int32 kLv4Index = 3;

	UPROPERTY()
	FTransform StartPlayerTransform;

	UPROPERTY(EditAnywhere,Instanced,Category = "Ghost")
	TObjectPtr<UAfterImageAttackEffect>  AfterImageAttackEffect;
};
