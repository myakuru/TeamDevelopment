// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerParameterData.generated.h"

/**
 * プレイヤーのパラメータデータを管理するクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerParameterData : public UDataAsset
{
	GENERATED_BODY()

public:

	UPlayerParameterData();

	/** プレイヤーの最大HPを取得するセッター */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetMaxHealth() const { return MaxHealth; }

protected:
	/** 最大HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/** 現在のギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float GearEnergy;

	/** 最大ギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxGearEnergy;
	
};
