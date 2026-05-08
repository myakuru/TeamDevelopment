// Fill out your copyright notice in the Description page of Project Settings.

#include "SuperGameInstance.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

void USuperGameInstance::Init()
{
	Super::Init();

	WeaponManager = NewObject<UWeaponManager>(this);
	if (WeaponManager) WeaponManager->Initialize(WeaponDataTable,WeaponMaterialDataTable);

	if (PlayerRuntimeData) {
		PlayerRuntimeData->Initialize();
	}

	LoadGameData();

}

void USuperGameInstance::LoadGameData()
{
	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	if (UGameplayStatics::DoesSaveGameExist(SlotName, UserIndex))
	{
		CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::LoadGameFromSlot(SlotName, UserIndex)
		);
	}
	else
	{
		CurrentSaveData = Cast<UMySaveGame>(
			UGameplayStatics::CreateSaveGameObject(UMySaveGame::StaticClass())
		);
	}

	if (WeaponManager) {
		WeaponManager->LoadFromSaveData(CurrentSaveData);
	}

}

void USuperGameInstance::SaveGameData()
{
	if (!CurrentSaveData)return;

	if (WeaponManager) {
		WeaponManager->SaveToData(CurrentSaveData);
	}

	const FString SlotName = UMySaveGame::GetSaveSlotName();
	const int32 UserIndex = 0;

	UGameplayStatics::SaveGameToSlot(CurrentSaveData, SlotName, UserIndex);

}
