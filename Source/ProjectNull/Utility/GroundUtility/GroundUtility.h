
#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GroundUtility.generated.h"

UCLASS()
class PROJECTNULL_API UGroundUtility : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
public:
	UGroundUtility();
public:

	static FQuat MakeRotationFromGroundNormal(
		const FTransform& InTransform,
		const FVector& InGroundNormal);
	
};
