
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GhostActorAnimInstance.generated.h"

UCLASS()
class PROJECTNULL_API UGhostActorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGhostActorAnimInstance();
public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPoseSnapshot GhostPoseSnapshot;
};
