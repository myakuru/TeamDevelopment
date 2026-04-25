// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperGameInstance.h"
#include "../Weapon/Manager/WeaponManager.h"
#include "../SaveGame/MySaveGame.h"
#include "Kismet/GameplayStatics.h"

void USuperGameInstance::Init()
{
	Super::Init();

	m_WeaponManager = NewObject<UWeaponManager>(this);


	// •Šíƒ[ƒh‚µ‚½‚ç“n‚·
	//if (m_WeaponManager) m_WeaponManager->Initialize();
}

void USuperGameInstance::LoadGameData()
{
	FString SlotName = "WeaponSave";
	int32 UserIndex = 0;

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
}

void USuperGameInstance::ApplySaveData()
{
	if (!m_CurrentSaveData || !m_WeaponManager)return;

	m_WeaponManager->Initialize(m_CurrentSaveData->m_Weapons, m_WeaponDataTable);

}
