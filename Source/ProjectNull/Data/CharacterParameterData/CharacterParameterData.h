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

	/** スキルのクールダウンを毎フレーム更新する */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void UpdateSkillCooldown(int32 SkillIndex, float DeltaTime);

	/** スキルのクールダウンをリセットする処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void ResetSkillCooldown(int32 SkillIndex);

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

	/** 現在の経験値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float Experience;

	/** 次のレベルまでの経験値 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	float MaxExperience;

	/** スキルの最大クールダウン時間（エディタで設定する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> SkillCooldownTime;

	/** スキルの経過クールダウン時間（ランタイムで管理する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> SkillCooldownElapsed;
};