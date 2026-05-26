// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../Instance/WeaponInstance.h"
#include "../Instance/WeaponMaterialInstance.h"
#include "../Data/WeaponData.h"
#include "WeaponManager.generated.h"

struct FWeaponMaterialData;
class UMySaveGame;

/**
 * 
 */
UCLASS(BlueprintType)
class PROJECTNULL_API UWeaponManager : public UObject
{
	GENERATED_BODY()
	
public:

	void Initialize(UDataTable* a_DTWeapon,UDataTable* a_DTMaterial);

	void SaveToData(UMySaveGame* a_SaveGame);

	void LoadFromSaveData(UMySaveGame* a_SaveGame);

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponInstance>& GetWeapons() const;

	UFUNCTION(BlueprintCallable)
	const TArray<FWeaponMaterialInstance>& GetMaterials()const;

	UFUNCTION(BlueprintCallable)
	void AddWeapon(const FWeaponInstance& a_NewWeapon);

	UFUNCTION(BlueprintCallable)
	void RemoveWeapon(int64 a_TargetUniqueID);

	UFUNCTION(BlueprintCallable)
	void AddWeaponMaterial(const FWeaponMaterialInstance& a_NewMaterial);

	UFUNCTION(BlueprintCallable)
	void RemoveWeaponMaterial(int64 a_TargetUniqueID);

	UFUNCTION(BlueprintCallable)
	bool GetWeaponMaster(FName a_WeaponId,FWeaponData& a_OutData)const;

	UFUNCTION(BlueprintCallable)
	bool GetMaterialMaster(FName a_MaterialID, FWeaponMaterialData& a_OutData)const;

	UFUNCTION(BlueprintCallable)
	void SetEquippedWeapon(int32 a_Index, const FWeaponInstance& a_Weapon);

	UFUNCTION(BlueprintCallable)
	bool GetEquippedWeapon(FWeaponInstance& a_EquippedWeapon, int32 a_Index);

	UFUNCTION(BlueprintCallable)
	UDataTable* GetWeaponDataTable();


private:

	UPROPERTY()
	TArray<FWeaponInstance> m_Weapons;

	UPROPERTY()
	int64 m_NextWeaponID = 0;

	UPROPERTY()
	TArray<FWeaponMaterialInstance> m_Materials;

	UPROPERTY()
	int64 m_NextMaterialID = 0;

	UPROPERTY()
	TArray<int64> m_EquippedWeaponIDs;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* m_WeaponDataTable;

	UPROPERTY(EditAnywhere, Category = "Data")
	UDataTable* m_MaterialDataTable;

};
