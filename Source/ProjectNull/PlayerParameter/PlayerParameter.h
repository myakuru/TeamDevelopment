// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerParameter.generated.h"

/**
 * プレイヤーのパラメータを管理するクラス (HP、ギアエネルギーなど)
 */
UCLASS()
class PROJECTNULL_API UPlayerParameter : public UObject
{
	GENERATED_BODY()

public:
	UPlayerParameter();

protected:
	/** 現在のHP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float Health;

	/** 最大HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float MaxHealth;

	/** 現在のギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float GearEnergy;

	/** 最大ギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float MaxGearEnergy;


public:
	//~ Begin Getter

	/** 現在のHPを取得 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetHealth() const { return Health; }

	/** 現在のギアエネルギーを取得 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetGearEnergy() const { return GearEnergy; }

	/** HPの割合を取得 (0~100) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetHealthRate() const;

	/** ギアエネルギーの割合を取得 (0~100) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetGearEnergyRate() const;

	//~ End Getter

	//~ Begin Setter

	/** HPを設定 (0~MaxHealth の範囲) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void SetHealth(float NewHealth);

	/** ギアエネルギーを設定 (0~MaxGearEnergy の範囲) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void SetGearEnergy(float NewGearEnergy);

	//~ End Setter

private:
	/** 割合計算の基準値 */
	static constexpr float PercentageBase = 100.0f;

};
