// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerParameter.h"

UPlayerParameter::UPlayerParameter()
	: Health(0.0f)
	, MaxHealth(0.0f)
	, GearEnergy(0.0f)
	, MaxGearEnergy(0.0f)
{
}

float UPlayerParameter::GetHealthRate() const
{
	// 0演算を防止
	if (MaxHealth <= 0.0f) return 0.0f;

	//  %表記にする
	return FMath::Clamp(Health / MaxHealth, 0.0f, 1.0f) * PercentageBase;
}

float UPlayerParameter::GetGearEnergyRate() const
{
	// 0演算を防止
	if (MaxGearEnergy <= 0.0f) return 0.0f;

	//  %表記にする
	return FMath::Clamp(GearEnergy / MaxGearEnergy, 0.0f, 1.0f) * PercentageBase;
}

void UPlayerParameter::SetHealth(float NewHealth)
{
	Health = FMath::Clamp(NewHealth, 0.0f, MaxHealth);
}

void UPlayerParameter::SetGearEnergy(float NewGearEnergy)
{
	GearEnergy = FMath::Clamp(NewGearEnergy, 0.0f, MaxGearEnergy);
}