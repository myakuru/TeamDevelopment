// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include"../Instance/WeaponInstance.h"
#include"../Data/WeaponData.h"
#include "WeaponManager.generated.h"

class UMySaveGame;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNULL_API UWeaponManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize(UDataTable* InDataTable);

	void SaveToData(UMySaveGame* a_SaveGame);

	void LoadFromSaveData(UMySaveGame* a_SaveGame);

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponInstance>& GetWeapons() const;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(const FWeaponInstance& a_NewWeapon);

	UFUNCTION(BlueprintCallable)
	bool GetWeaponMaster(FName WeaponId,FWeaponData& OutData)const;

	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(int32 a_Index, const FWeaponInstance& a_Weapon);

	UFUNCTION(BlueprintCallable)
	bool GetEquippedWeapon(FWeaponInstance& a_EquippedWeapon, int32 a_Index);

private:

	UPROPERTY()
	TArray<FWeaponInstance> m_Weapons;

	UPROPERTY()
	int64 m_NextWeaponID = 0;

	UPROPERTY()
	TArray<int64> m_EquippedWeaponIDs;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* WeaponDataTable;
};
