
#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "PlayerRuntimeData.generated.h"

/** 経験値が変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, int32, NewExperience, int32, MaxExperience);

/** HPが変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnHealthChanged, float, NewHealth, float, MaxHealth);

/** ギアエネルギーが変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnGearEnergyChanged, float, GearEnergy);

/** 経験値関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FExperienceRuntimeData
{
	GENERATED_BODY()

public:

	FExperienceRuntimeData() :
		Current(0.0f),
		ExperienceToNextLevel(10.0f),
		Total(0.0f)
	{
	}

	/**
	 * @brief 経験値加算処理
	 * @param Amount 加算量
	 */
	inline void Add(float Amount)
	{
		Current += Amount;
		Total += Amount;
	}

	/**
	 * @brief 現在の経験値でレベルアップ可能か判定
	 * @return レベルアップ可能ならtrue レベルアップ不可能なら false
	 */
	inline bool CanLevelUp() const
	{
		return Current >= ExperienceToNextLevel;
	}

	/**
	 * @brief 次レベルまでの必要経験値の計算処理
	 * @param BaseValue 次レベルに必要な基礎経験値
	 * @param ToNextLevelIncreasePerLevelValue 次レベル必要経験値のレベルごとの増加量
	 * @param Level キャラクターのレベル
	 */
	inline void CalculateExperienceToNextLevel(
		float BaseValue,
		float ToNextLevelIncreasePerLevelValue,
		int32 Level)
	{
		ExperienceToNextLevel = BaseValue + ToNextLevelIncreasePerLevelValue * Level;
	}

	/** 現在の経験値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	float Current;

	/** 次レベルまでの必要経験値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Experience")
	float ExperienceToNextLevel;

	/** 合計経験値 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Experience")
	float Total;
};

/** 速度関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FSpeedRuntimeData
{
	GENERATED_BODY()

public:

	FSpeedRuntimeData() :
		Final(0.0f)
	{
	}

public:
	/** 最終的なプレイヤーの速度 = (基礎速度 + 速度増加量 * プレイヤーレベル) * ギア段階による速度倍率 */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Speed")
	float Final;
};

/** ギア関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FGearRuntimeData
{
	GENERATED_BODY()

public:

	FGearRuntimeData()
		: GearEnergy(0.0f)
	{
	}

public:
	/** 現在のギアエネルギー */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Gear")
	float GearEnergy;
};

/** プレイヤー中間基底クラス */
class APlayerBase;

/** 速度パラメータ構造体 */
struct FSpeedParameterData;

/** 経験値パラメータ構造体 */
struct FExperienceParameterData;


/**
 * プレイヤーのRuntimeデータクラス
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UPlayerRuntimeData final : public UCharacterRuntimeData
{
	GENERATED_BODY()

public:
	UPlayerRuntimeData();

public:
	void Initialize() override;

	/** 経験値が変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnExperienceChanged OnExperienceChanged;

	/** HPが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	/** ギアエネルギーが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnGearEnergyChanged OnGearEnergyChanged;

	/** セッター */
	inline void SetOwner(APlayerBase* SetOwner) { Owner = SetOwner; }
	
	/**
	 * @brief プレイヤーの経験値加算処理
	 * @param Amount 加算される経験値
	 */
	void AddExperience(float Amount);

private:

	/**
	 * @brief レベルアップ処理
	 */
	void LevelUp();

	/**
	 * @brief 最終的な経験値計算処理
	 */
	void CalculateExperience(const FExperienceParameterData& Data);

	/**
	 * @brief 最終的な速度計算処理
	 */
	void CalculateFinalSpeed(const FSpeedParameterData& Data,int32 CurrentGearLevel);

	/**
	 * @brief 計算済みの速度をCharacterMovementに適用する
	 */
	void ApplyMovementSpeed();

	/**
	 * @brief レベルアップ時のプレイヤーステータスを更新（計算、適用）
	 */
	void UpdateStatus();
	
	/** 持ち主のクラス */
	UPROPERTY()
	APlayerBase* Owner;

	/** 経験値関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Experience")
	FExperienceRuntimeData Experience;

	/** 速度関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Speed")
	FSpeedRuntimeData Speed;

	/** ギア関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Gear")
	FGearRuntimeData Gear;

	/** レベル */
	int32 Level;
};
