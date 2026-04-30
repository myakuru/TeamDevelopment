#pragma once

#include "CoreMinimal.h"
#include "../ItemBase.h"
#include "PickupBase.generated.h"

UCLASS()
class PROJECTNULL_API APickupBase : public AItemBase
{
	GENERATED_BODY()

public:

	APickupBase();

protected:

	virtual void BeginPlay()override {}
	//virtual void EndPlay()override {}

	/** çXêV*/
	void OnUpdate(APawn* Player, float DeltaTime) override {}

};
