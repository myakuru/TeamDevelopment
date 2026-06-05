#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "HealGearStateBase.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UHealGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
	
public:
	UHealGearStateBase();

public:

	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;

protected:

	void Heal();

	UPROPERTY(EditAnywhere)
	float HealAmount = 10.0f;
};
