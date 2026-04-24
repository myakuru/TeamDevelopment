// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerParameter.generated.h"

/**
 * �v���C���[�̃p�����[�^�iHP�E�M�A�G�l���M�[�j��Ǘ�����N���X
 */
UCLASS()
class PROJECTNULL_API UPlayerParameter : public UObject
{
	GENERATED_BODY()

public:

	UPlayerParameter();

protected:
	/** ���݂�HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float Health;

	/** �ő�HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float MaxHealth;

	/** �M�A�G�l���M�[�̌��ݒl */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float GearEnergy;

	/** �M�A�G�l���M�[�̍ő�l */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter")
	float MaxGearEnergy;


public:
	//~ Begin Getter

	/** ���݂�HP��Ԃ� */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetHealth() const { return Health; }

	/** ���݂̃M�A�G�l���M�[��Ԃ� */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetGearEnergy() const { return GearEnergy; }

	/** HP�̊�����Ԃ� (0~100) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetHealthRate() const;

	/** �M�A�G�l���M�[�̊�����Ԃ� (0~100) */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float GetGearEnergyRate() const;

	//~ End Getter

	//~ Begin Setter

	/** HP��ݒ肷��i0~MaxHealth �ɃN�����v�j */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void SetHealth(float NewHealth);

	/** �M�A�G�l���M�[��ݒ肷��i0~MaxGearEnergy �ɃN�����v�j */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void SetGearEnergy(float NewGearEnergy);

	//~ End Setter

private:
	/** �p�[�Z���g�ϊ��p�萔 */
	static constexpr float PercentageBase = 100.0f;

};
