// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "ExplosionGearStateBase.generated.h"

class AExplosionGearSkill;

USTRUCT(BlueprintType)
struct FExplosionSpawnData 
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FVector Offset = FVector::ZeroVector;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	float Scale = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Damage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Delay;
};

UCLASS()
class PROJECTNULL_API UExplosionGearStateBase : public UGearStateBase
{
	GENERATED_BODY()

public:
	UExplosionGearStateBase();
public:

	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;

protected:

	UPROPERTY(EditAnywhere)
	TSubclassOf<AExplosionGearSkill> ExplosionClass;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FExplosionSpawnData> ExplosionDatas;

	void SpawnExplosions();

};
