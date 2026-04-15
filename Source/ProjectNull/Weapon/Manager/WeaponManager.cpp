// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"

void UWeaponManager::Initialize(const TArray<FWeaponInstance>& InWeapons,UDataTable* InDataTable)
{
	m_Weapons = InWeapons;
    WeaponDataTable = InDataTable;
}

const TArray<FWeaponInstance>& UWeaponManager::GetWeapons() const
{
	return m_Weapons;
}

void UWeaponManager::AddWeapon(const FWeaponInstance& NewWeapon)
{
	m_Weapons.Add(NewWeapon);
}

bool UWeaponManager::GetWeaponMaster(FName WeaponId, FWeaponData& OutData) const
{
    if (!WeaponDataTable)return false;
    FWeaponData* FoundData = WeaponDataTable->FindRow<FWeaponData>(WeaponId, TEXT(""));
    //if (!FoundData)return false;


    OutData = *FoundData;
    return true;
}