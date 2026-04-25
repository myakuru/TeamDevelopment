// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Weapon/Instance/WeaponInstance.h"

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

	UPROPERTY()
	TArray<FWeaponInstance> m_Weapons;
	
};
