#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

UCLASS()
class PROJECTNULL_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPlayerAnimInstance();

	/** 戦闘構え状態か */
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatStance;

};
