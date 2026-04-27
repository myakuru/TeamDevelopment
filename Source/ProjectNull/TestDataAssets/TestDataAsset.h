// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "TestDataAsset.generated.h"

USTRUCT()
struct FTestInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString Name;

	UPROPERTY(EditAnywhere)
	UTestObj* TestObj;
};


UCLASS(Abstract,BlueprintType,EditInlineNew,DefaultToInstanced)
class PROJECTNULL_API UTestObj : public UObject
{
	GENERATED_BODY()
};

UCLASS(BlueprintType)
class PROJECTNULL_API UAttack : public UTestObj
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Params1;
};

UCLASS(BlueprintType)
class PROJECTNULL_API USpeed : public UTestObj
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere)
	float Params2;
};

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UTestDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere,Instanced)
	TArray<UTestObj*> TeseInfo;
};