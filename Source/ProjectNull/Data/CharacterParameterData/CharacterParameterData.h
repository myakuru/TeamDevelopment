// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CharacterParameterData.generated.h"

/** HPが変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

/** ギアエネルギーが変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearEnergyChanged, float, GearEnergy);

/** 経験値が変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, int32, NewExperience, int32, MaxExperience);

/** スキルのクールダウン時間が変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillCooldownChanged, int32, SkillIndex, float, CooldownTime, float, CooldownRunTime);

/**
 * CharacterのHPなどのデータを管理するクラス
 */
UCLASS()
class PROJECTNULL_API UCharacterParameterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UCharacterParameterData();

	//~ Begin Setter

	/** HPを減算する処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void DecreaseHealth(float Amount);

	/** ギアエネルギーを加算する処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void AddGearEnergy(float Amount);

	/** 経験値を加算する処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void AddExperience(float Amount);

	/** 最終的な速度計算処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	float CalculateFinalSpeed();

	/** スキルのクールダウンを毎フレーム更新する */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void UpdateSkillCooldown(int32 SkillIndex, float DeltaTime);

	/** スキルのクールダウンをリセットする処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void ResetSkillCooldown(int32 SkillIndex);

	inline void SetCurrentGearLevel(int32 a_CurrentGearLevel) { CurrentGearLevel = a_CurrentGearLevel; }
	//~ End Setter


	/** HPが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	/** ギアエネルギーが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnGearEnergyChanged OnGearEnergyChanged;

	/** 経験値が変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnExperienceChanged OnExperienceChanged;

	/** スキルのクールダウン時間が変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnSkillCooldownChanged OnSkillCooldownChanged;

private:

	/** 最終的な経験値計算処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void LevelUp();

	/** 最終的な経験値計算処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void CalculateExperience();

	/** 現在のHP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float Health;

	/** 最大HP */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/** 現在のギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float GearEnergy;

	/** 最大ギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxGearEnergy;

	/** 次のレベルまでの必要経験値 = base + 必要経験値増加量 * プレイヤーのレベル */

	/** 次レベルに必要な基礎経験値（レベル補正前） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float BaseExperienceToNextLevel;

	/** 現在の経験値 */
	float CurrentExperience;

	/** 次レベルまでの必要経験値 */
	float ExperienceToNextLevel;

	/** 次レベル必要経験値のレベルごとの増加量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float ExperienceToNextLevelIncreasePerLevel;

	/** 合計経験値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float TotalExperience;

	/** 次のレベルまでの経験値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxExperience;

	/** プレイヤーのレベル */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	int32 Level;
	
	/** 最終的なプレイヤーの速度 = (基礎速度 + 速度増加量 * プレイヤーレベル) * ギア段階による速度倍率 */
	float FinalSpeed;

	/** プレイヤーの基礎速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float BaseSpeed;

	/** レベルに応じての速度増加量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float ScalePerLevelSpeed;

	/** ギアレベル*/
	int32 CurrentGearLevel;

	/** ギアレベルに応じた速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> GearLevelSpeedMultiplierArray;

	/** スキルの最大クールダウン時間（エディタで設定する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> SkillCooldownTime;

	/** スキルの経過クールダウン時間（ランタイムで管理する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> SkillCooldownElapsed;
};