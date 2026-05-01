#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OB_KitchenSubjectInterface.generated.h"

/**
 * 
 */
UINTERFACE(MinimalAPI,BlueprintType)
class UOB_KitchenSubjectInterface:public UInterface
{
	GENERATED_BODY()
};


class PROJECTNULL_API IOB_KitchenSubjectInterface
{

	GENERATED_BODY()

public:

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Observer")
	void AddObserver(const TScriptInterface<class IOB_KitchenSubjectInterface>& a_Observer);
};