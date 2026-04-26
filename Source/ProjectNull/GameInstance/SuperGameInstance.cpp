// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperGameInstance.h"
#include "../Weapon/Manager/WeaponManager.h"
#include "../SaveGame/MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void USuperGameInstance::Init()
{
	Super::Init();

	m_WeaponManager = NewObject<UWeaponManager>(this);
	if (m_WeaponManager) m_WeaponManager->Initialize(m_WeaponDataTable);

	LoadGameData();

}

void USuperGameInstance::LoadGameData()
{
	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		m_CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)
		);
	}
	else
	{
		m_CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())
		);
	}

	if (m_WeaponManager) {
		m_WeaponManager->LoadFromSaveData(m_CurrentSaveData);
	}

}

void USuperGameInstance::SaveGameData()
{
	if (!m_CurrentSaveData)return;

	if (m_WeaponManager) {
		m_WeaponManager->SaveToData(m_CurrentSaveData);
	}

	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	UGameplayStatics::SaveGameToSlot(m_CurrentSaveData, SlotName, UserIndex);

}
