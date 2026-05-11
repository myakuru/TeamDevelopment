#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SuperGameInstance.generated.h"

class UWeaponManager;
class UMySaveGame;
class UPlayerParameterData;
class UPlayerRuntimeData;

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
	inline TObjectPtr<UPlayerParameterData> GetPlayerParameterData() const { return PlayerParameterData; }
	inline TObjectPtr<UPlayerRuntimeData> GetPlayerRuntimeData() const { return PlayerRuntimeData; }

protected:

	UPROPERTY()
	UMySaveGame* CurrentSaveData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* WeaponDataTable;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UDataTable* WeaponMaterialDataTable;

	UPROPERTY(BlueprintReadOnly)
	UWeaponManager* WeaponManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UPlayerParameterData> PlayerParameterData;

	/** Runtimeデータ 実行時に変更されるデータを計算、適用するクラス */
	UPROPERTY(EditAnywhere, Instanced)
	TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData;
};
