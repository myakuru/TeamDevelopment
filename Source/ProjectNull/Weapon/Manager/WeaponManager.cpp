// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"
#include"../../SaveGame/MySaveGame.h"
#include"../Data/WeaponMaterialData.h"

void UWeaponManager::Initialize()
{
	m_EquippedWeaponIDs.Add(-1);
	m_EquippedWeaponIDs.Add(-1);
	m_EquippedWeaponIDs.Add(-1);
}

void UWeaponManager::SaveToData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

	a_SaveGame->m_WeaponData.NextWeaponUniqueID = m_NextWeaponID;

    a_SaveGame->m_WeaponData.Weapons.Empty();
    for (const FWeaponInstance& Weapon : m_Weapons) {
        a_SaveGame->m_WeaponData.Weapons.Add(Weapon);
    }

	a_SaveGame->m_WeaponData.EquippedWeaponIDs = m_EquippedWeaponIDs;

	a_SaveGame->m_WeaponData.NextMaterialUniqueID = m_NextMaterialID;

	a_SaveGame->m_WeaponData.Materials.Empty();
	for (const FWeaponMaterialInstance& Material : m_Materials) {
		a_SaveGame->m_WeaponData.Materials.Add(Material);
	}

}

void UWeaponManager::LoadFromSaveData(UMySaveGame* a_SaveGame)
{
    if (!a_SaveGame)return;

	m_NextWeaponID = a_SaveGame->m_WeaponData.NextWeaponUniqueID;

    m_Weapons.Empty();
	m_Weapons = a_SaveGame->m_WeaponData.Weapons;

	if (!a_SaveGame->m_WeaponData.EquippedWeaponIDs.IsEmpty()) {
		m_EquippedWeaponIDs.Empty();
		m_EquippedWeaponIDs = a_SaveGame->m_WeaponData.EquippedWeaponIDs;
	}

	m_NextMaterialID = a_SaveGame->m_WeaponData.NextMaterialUniqueID;

	m_Materials.Empty();
	m_Materials = a_SaveGame->m_WeaponData.Materials;

}

const TArray<FWeaponInstance>& UWeaponManager::GetWeapons() const
{
	return m_Weapons;
}

const TArray<FWeaponMaterialInstance>& UWeaponManager::GetMaterials() const
{
	return m_Materials;
}

void UWeaponManager::AddWeapon(const FWeaponInstance& a_NewWeapon)
{
	m_Weapons.Add(a_NewWeapon);
	m_Weapons.Last().UniqueId = m_NextWeaponID;
	m_NextWeaponID++;
}

void UWeaponManager::RemoveWeapon(int64 a_TargetUniqueID)
{
	const int32 index = m_Weapons.IndexOfByPredicate
	(
		[a_TargetUniqueID](const FWeaponInstance& Weapon)
		{
			return Weapon.UniqueId == a_TargetUniqueID;
		}
	);

	if (index != INDEX_NONE) {
		m_Weapons.RemoveAt(index);
	}

}

void UWeaponManager::AddWeaponMaterial(const FWeaponMaterialInstance& a_NewMaterial)
{
	m_Materials.Add(a_NewMaterial);
	m_Materials.Last().UniqueId = m_NextMaterialID;
	m_NextMaterialID++;
}

void UWeaponManager::RemoveWeaponMaterial(int64 a_TargetUniqueID)
{
	const int32 index = m_Materials.IndexOfByPredicate
	(
		[a_TargetUniqueID](const FWeaponMaterialInstance& Material)
		{
			return Material.UniqueId == a_TargetUniqueID;
		}
	);

	if (index != INDEX_NONE) {
		m_Materials.RemoveAt(index);
	}

}

bool UWeaponManager::GetWeaponMaster(FName a_WeaponId, FWeaponData& a_OutData) const
{
    if (!m_WeaponDataTable)return false;
    FWeaponData* FoundData = m_WeaponDataTable->FindRow<FWeaponData>(a_WeaponId, TEXT(""));

    if (!FoundData)return false;
    a_OutData = *FoundData;
    return true;
}

bool UWeaponManager::GetMaterialMaster(FName a_MaterialID, FWeaponMaterialData& a_OutData) const
{
	if (!m_MaterialDataTable)return false;
	FWeaponMaterialData* FoundData = m_MaterialDataTable->FindRow<FWeaponMaterialData>(a_MaterialID, TEXT(""));

	if (!FoundData)return false;
	a_OutData = *FoundData;
	return true;
}

void UWeaponManager::SetEquippedWeapon(int32 a_Index, const FWeaponInstance& a_Weapon)
{
	if (!m_EquippedWeaponIDs.IsValidIndex(a_Index))return;

	for (int i = 0; i < m_EquippedWeaponIDs.Num();i++) {
		if (m_EquippedWeaponIDs[i] == a_Weapon.UniqueId) {
			m_EquippedWeaponIDs[i] = INDEX_NONE;
		}
	}
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

UDataTable* UWeaponManager::GetWeaponDataTable()
{
	if (m_WeaponDataTable) return m_WeaponDataTable;
	else return nullptr;
}

