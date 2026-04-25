// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Weapon/SaveData/WeaponSaveData.h"

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MySaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECTNULL_API UMySaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	static const FString& GetSaveSlotName()
	{
		static const FString Name = TEXT("MainSave");
		return Name;
	}

	UPROPERTY()
	FWeaponSaveData m_WeaponData;
};
