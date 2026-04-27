// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UEffectBase.generated.h"

class APlayerBase;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UUEffectBase : public UObject
{
	GENERATED_BODY()

public:

	virtual void Apply(APlayerBase* a_Player) {}


};
