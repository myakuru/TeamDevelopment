// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponManager.h"

void UWeaponManager::Initialize(const TArray<FWeaponInstance>& InWeapons)
{
	m_Weapons = InWeapons;
}

const TArray<FWeaponInstance>& UWeaponManager::GetWeapons() const
{
	return m_Weapons;
}

void UWeaponManager::AddWeapon(const FWeaponInstance& NewWeapon)
{
	m_Weapons.Add(NewWeapon);
}
