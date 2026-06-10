#pragma once

#include "CoreMinimal.h"
#include "../GearBase.h"
#include "HealGear.generated.h"

/** 回復ギアクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UHealGear : public UGearBase
{
	GENERATED_BODY()
public:
	UHealGear();
public:

	void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent)	override;
	void Execute(int32 CurrentGearLevel)													override;
	void Update(float DeltaTime)															override;
	
};
