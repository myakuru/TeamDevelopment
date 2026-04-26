// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SuperGameInstance.generated.h"

class UWeaponManager;
class UMySaveGame;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API USuperGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init()override;

	UPROPERTY()
	UMySaveGame* m_CurrentSaveData;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* m_WeaponDataTable;
	
	UPROPERTY(BlueprintReadOnly)
	UWeaponManager* m_WeaponManager;

	UFUNCTION(BlueprintCallable)
	void LoadGameData();

	UFUNCTION(BlueprintCallable)
	void SaveGameData();

private:
	

};
