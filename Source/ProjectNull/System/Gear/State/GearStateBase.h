
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearStateBase.generated.h"

class UPlayerGearComponent;

class APlayerBase;

class UGearBase;

UCLASS()
class PROJECTNULL_API UGearStateBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearStateBase();

public:

	virtual void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent,class UGearBase* Gear);

	virtual void Execute(int32 CurrentGearLevel) { return; }

	virtual void Update(float DeltaTime) { return; }

	virtual void End() { return; }

protected:

	UPROPERTY()
	APlayerBase* OwnerPlayer;

	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

	UPROPERTY()
	UGearBase* OwnerGear;

};
