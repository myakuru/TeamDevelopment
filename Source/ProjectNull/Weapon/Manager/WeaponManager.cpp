// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include"../../SaveGame/MySaveGame.h"

void UWeaponManager::Initialize(UDataTable* InDataTable)
{
    WeaponDataTable = InDataTable;
}

void UWeaponManager::SaveToData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

    a_SaveGame->m_WeaponData.Weapons.Empty();

    for (const FWeaponInstance& Weapon : m_Weapons) {
        a_SaveGame->m_WeaponData.Weapons.Add(Weapon);
    }

}

void UWeaponManager::LoadFromSaveData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

    m_Weapons.Empty();

    for (const FWeaponInstance& Data : a_SaveGame->m_WeaponData.Weapons)
    {
        m_Weapons.Add(Data);
    }

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