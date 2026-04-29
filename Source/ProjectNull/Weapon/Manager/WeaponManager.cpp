// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include"../../SaveGame/MySaveGame.h"

void UWeaponManager::Initialize(UDataTable* InDataTable)
{
    WeaponDataTable = InDataTable;
	m_EquippedWeaponIDs.Add(-1);
	m_EquippedWeaponIDs.Add(-1);
	m_EquippedWeaponIDs.Add(-1);
}

void UWeaponManager::SaveToData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

	a_SaveGame->m_WeaponData.NextUniqueID = m_NextWeaponID;

    a_SaveGame->m_WeaponData.Weapons.Empty();
    for (const FWeaponInstance& Weapon : m_Weapons) {
        a_SaveGame->m_WeaponData.Weapons.Add(Weapon);
    }

	a_SaveGame->m_WeaponData.EquippedWeaponIDs = m_EquippedWeaponIDs;

}

void UWeaponManager::LoadFromSaveData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

	m_NextWeaponID = a_SaveGame->m_WeaponData.NextUniqueID;

    m_Weapons.Empty();
	m_Weapons = a_SaveGame->m_WeaponData.Weapons;

	if (!a_SaveGame->m_WeaponData.EquippedWeaponIDs.IsEmpty()) {
		m_EquippedWeaponIDs = a_SaveGame->m_WeaponData.EquippedWeaponIDs;
	}

}

const TArray<FWeaponInstance>& UWeaponManager::GetWeapons() const
{
	return m_Weapons;
}

void UWeaponManager::AddWeapon(const FWeaponInstance& a_NewWeapon)
{
	m_Weapons.Add(a_NewWeapon);
	m_Weapons.Last().UniqueId = m_NextWeaponID;
	m_NextWeaponID++;
}

bool UWeaponManager::GetWeaponMaster(FName WeaponId, FWeaponData& OutData) const
{
    if (!WeaponDataTable)return false;
    FWeaponData* FoundData = WeaponDataTable->FindRow<FWeaponData>(WeaponId, TEXT(""));

    if (!FoundData)return false;
    OutData = *FoundData;
    return true;
}

void UWeaponManager::SetEquippedWeapon(int32 a_Index, const FWeaponInstance& a_Weapon)
{
	if (!m_EquippedWeaponIDs.IsValidIndex(a_Index))return;

	m_EquippedWeaponIDs[a_Index] = a_Weapon.UniqueId;

}

bool UWeaponManager::GetEquippedWeapon(FWeaponInstance& a_EquippedWeapon, int32 a_Index)
{
	if (!m_EquippedWeaponIDs.IsValidIndex(a_Index))return false;

	for (const FWeaponInstance& weapon : m_Weapons) {
		if (weapon.UniqueId == m_EquippedWeaponIDs[a_Index]) {
			a_EquippedWeapon = weapon;
			return true;
		}
	}

	return false;
}
