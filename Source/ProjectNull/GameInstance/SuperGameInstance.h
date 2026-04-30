// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SuperGameInstance.generated.h"

class UWeaponManager;
class UMySaveGame;
class UCharacterParameterData;

/**
 * ゲーム全体で共有されるデータや機能を管理するクラス
 */
UCLASS()
class PROJECTNULL_API USuperGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	virtual void Init()override;

	UFUNCTION(BlueprintCallable)
	void LoadGameData();

	UFUNCTION(BlueprintCallable)
	void SaveGameData();

	/** パラメーターを取得する */
	TObjectPtr<UCharacterParameterData> GetCharacterParameterData() const { return CharacterParameterData; }

protected:

	UPROPERTY()
	UMySaveGame* m_CurrentSaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* m_WeaponDataTable;

	UPROPERTY(BlueprintReadOnly)
	UWeaponManager* m_WeaponManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UCharacterParameterData> CharacterParameterData;

};
