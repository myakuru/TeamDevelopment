// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "PlayerParameterData.generated.h"

/** スキルのクールダウン時間が変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSkillCooldownChanged, int32, SkillIndex, float, CooldownTime, float, CooldownRunTime);

/** 経験値関連パラメータデータ構造体 */
USTRUCT(BlueprintType)
struct FExperienceParameterData
{
	GENERATED_BODY()

public:

	FExperienceParameterData() :
		BaseExperienceToNextLevel(0.0f),
		ExperienceToNextLevelIncreasePerLevel(0.0f)
	{
	}

public:
	/** 次のレベルまでの必要経験値 = base + 必要経験値増加量 * プレイヤーのレベル */

	/** 次レベルに必要な基礎経験値（レベル補正前） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	float BaseExperienceToNextLevel;


	/** 次レベル必要経験値のレベルごとの増加量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Experience")
	float ExperienceToNextLevelIncreasePerLevel;
};

/** 速度関連パラメータデータ構造体 */
USTRUCT(BlueprintType)
struct FSpeedParameterData
{
	GENERATED_BODY()

public:

	FSpeedParameterData() :
		Base(0.0f),
		ScalePerLevelSpeed(0.0f),
		GearLevelSpeedMultiplierArray(TArray<float>())
	{
	}

public:

	/** プレイヤーの基礎速度 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float Base;

	/** レベルに応じての速度増加量 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float ScalePerLevelSpeed;

	/** ギアレベルに応じた速度倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Speed")
	TArray<float> GearLevelSpeedMultiplierArray;
};

/**　ギア関連パラメータデータ構造体 */
USTRUCT(BlueprintType)
struct FGearParameterData
{
	GENERATED_BODY()

public:

	FGearParameterData() :
		BaseInvincibilityTime(0.0f),
		ExtraInvincibilityTimePerExcessRatio(0.0f),
		InvincibilityCollisionRadiusSquared(0.0f)
	{
	}

public:

	/** 基礎無敵時間 */
	UPROPERTY(EditAnywhere, Category = "Gear")
	float BaseInvincibilityTime;

	/** 超過割合による無敵時間増加量 */
	UPROPERTY(EditAnywhere, Category = "Gear")
	float ExtraInvincibilityTimePerExcessRatio;

	/** 無敵状態用当たり判定半径の二乗値 */
	UPROPERTY(EditAnywhere, Category = "Gear")
	float InvincibilityCollisionRadiusSquared;
};

/**
 * CharacterのHPなどのデータを管理するクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerParameterData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPlayerParameterData();

	//~ Begin Setter

	/** スキルのクールダウンを毎フレーム更新する */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void UpdateSkillCooldown(int32 SkillIndex, float DeltaTime);

	/** スキルのクールダウンをリセットする処理 */
	UFUNCTION(BlueprintCallable, Category = "PlayerParameter")
	void ResetSkillCooldown(int32 SkillIndex);

	//~ End Setter

	inline FExperienceParameterData GetExperienceData() const { return Experience; }
	inline FSpeedParameterData GetSpeedData() const { return Speed; }
	inline FGearParameterData GetGearData() const { return Gear; }

	/** スキルのクールダウン時間が変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnSkillCooldownChanged OnSkillCooldownChanged;

private:
	/** 経験値関連パラメータデータ構造体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	FExperienceParameterData Experience;
	
	/** 速度関連パラメータデータ構造体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	FSpeedParameterData Speed;

	/** ギア関連パラメータデータ構造体 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	FGearParameterData Gear;

	/** スキルの最大クールダウン時間（エディタで設定する） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerParameter", meta = (AllowPrivateAccess = "true"))
	TArray<float> SkillCooldownTime;

	/** スキルの経過クールダウン時間（ランタイムで管理する） */
	TArray<float> SkillCooldownElapsed;
};