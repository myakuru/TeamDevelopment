
#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "PlayerRuntimeData.generated.h"

/** 経験値関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FExperienceRuntimeData
{
	GENERATED_BODY()

public:

	FExperienceRuntimeData() :
		Current(0.0f),
		ExperienceToNextLevel(0.0f),
		Total(0.0f)
	{
	}

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
		Current >= ExperienceToNextLevel;
	}

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

/** プレイヤー中間基底クラス */
class APlayerBase;

struct FSpeedParameterData;
struct FExperienceParameterData;

/**
 プレイヤーのRuntimeデータクラス
 */
UCLASS()
class PROJECTNULL_API UPlayerRuntimeData final : public UCharacterRuntimeData
{
	GENERATED_BODY()

public:
	UPlayerRuntimeData();

public:
	void Initialize() override;

	void SetOwner(const TObjectPtr<APlayerBase>& SetOwner) { Owner = SetOwner; }
	
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


	UPROPERTY()
	TObjectPtr<APlayerBase> Owner;

	/** 経験値関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Experience")
	FExperienceRuntimeData Experience;

	/** 速度関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Speed")
	FSpeedRuntimeData Speed;

	/** レベル */
	int32 Level;

};
