// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include"../Instance/WeaponInstance.h"
#include "WeaponManager.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNULL_API UWeaponManager : public UObject
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void Initialize(const TArray<FWeaponInstance>& InWeapons);

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponInstance>& GetWeapons() const;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(const FWeaponInstance& NewWeapon);

private:

	UPROPERTY()
	TArray<FWeaponInstance> m_Weapons;
};
