
#pragma once

#include "CoreMinimal.h"
#include "../CharacterRuntimeData.h"
#include "PlayerRuntimeData.generated.h"

/** 経験値が変更されたときに呼び出されるデリゲート */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnExperienceChanged, float, NewExperience, float, MaxExperience);

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

struct FUpgradeState
{
	FName UpgradeId;

	int32 Level = 0;

};

/** ギア関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FGearRuntimeData
{
	GENERATED_BODY()

public:

	FGearRuntimeData():
		GearEnergy(0.0f),
		GearChangeEnergyCost(TArray<float>())
	{
	}

public:

	inline bool CanChangeGear(int32 CurrentGearLevel)
	{
		const int32 Index = --CurrentGearLevel;
		if (!GearChangeEnergyCost.IsValidIndex(Index))	{ return false; }
		if (GearEnergy < GearChangeEnergyCost[Index])	{ return false; }
		return true;
	}

	/**
	 * @brief ギアチェンジ時にデータをリセット
	 * @param CurrentGearLevel 現在のギアレベル
	 */
	inline void ResetDataOnGearChange(int32 CurrentGearLevel)
	{
		const int32 Index = --CurrentGearLevel;
		if (!GearChangeEnergyCost.IsValidIndex(Index)) { return; }
		const float EnergyCost = GearChangeEnergyCost[Index];
		UE_LOG(LogTemp, Warning, TEXT("hi EnergyCost %.0f"), EnergyCost);
		ExcessRatio = (GearEnergy / EnergyCost) - 1.0f;
		UE_LOG(LogTemp, Warning, TEXT("hi ExcessRatio %.2f"), ExcessRatio);

		GearEnergy -= EnergyCost;

	}
	
	/**
	 * @brief 無敵時間を計算する
	 */
	inline void CalculateInvincibilityTime(float BaseTime,float ExtraInvincibilityTimePerExcessRatio)
	{
		GearChangeInvincibilityTime = BaseTime + ExcessRatio * ExtraInvincibilityTimePerExcessRatio;
	}

	/** 現在のギアエネルギー */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Gear")
	float GearEnergy;

	/** ギアチェンジに必要エネルギー */
	UPROPERTY(EditAnywhere, Category = "Gear")
	TArray<float> GearChangeEnergyCost;

	/** ギアチェンジによる無敵時間
	* 無敵時間 = 基礎無敵時間 + 超過した割合(124%なら24%) * 超過した無敵時間増加量 */
	float GearChangeInvincibilityTime;

	/** 超過割合（124% → 0.24f）*/
	float ExcessRatio;
};

/** プレイヤー中間基底クラス */
class APlayerBase;

/** ロボットコントローラークラス */
class ARobotController;

/** 強化画面のウィジット */
class UPlayerExpUpgradeWidget;

/** 速度パラメータ構造体 */
struct FSpeedParameterData;

/** 経験値パラメータ構造体 */
struct FExperienceParameterData;

/** ギアパラメータ構造体 */
struct FGearParameterData;

struct FUpgradeState;

/** プレイヤーのRuntimeデータクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UPlayerRuntimeData final : public UCharacterRuntimeData
{
	GENERATED_BODY()

public:
	UPlayerRuntimeData();

public:
	void Initialize() override;

	/**
	 * @brief プレイヤーの経験値加算処理
	 * @param Amount 加算される経験値
	 */
	void AddExperience(float Amount);

	/**
	 * @brief プレイヤーのギアエネルギー加算処理
	 * @param Amount 加算されるギアエネルギー
	 */
	void AddGearEnergy(float Amount);

	bool CanChangeGear(int32 CurrentGearLevel);

	void ResetDataOnGearChange(int32 CurrentGearLevel);

	void CalculateInvincibilityTime(const FGearParameterData& Data);

	/**
	 * @brief レベルアップ処理
	 */
	void LevelUp();

	inline void SetIsInvincible(bool SetFlg) { bIsInvincible = SetFlg; }

	inline bool IsInvincible() const { return bIsInvincible; }
	inline FGearRuntimeData& GetGearData() { return Gear; }

	/** 経験値が変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnExperienceChanged OnExperienceChanged;

	/** HPが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnHealthChanged OnHealthChanged;

	/** ギアエネルギーが変更されたときに呼び出されるデリゲート */
	UPROPERTY(BlueprintAssignable)
	FOnGearEnergyChanged OnGearEnergyChanged;


	TObjectPtr<ARobotController> RobotController;

	/** Widget側で呼び出す */
	void UpdateUpgradeStates(FName Id);

	int32 GetUpgradeLevel(FName Id) const;

private:

	

	/**
	 * @brief HPの更新処理
	 * @param NewHealth 初期値はPlayerParameterDataから受け取る。
	 * @param MaxHealth DataAssetのMaxHealthを渡すことで、UIのHPバーの割合を正しく表示できるようにする。
	 */
	void UpdateHealth(float NewHealth);

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

	TArray<FUpgradeState> UpgradeStates;

	UPROPERTY()
	UDataTable* CachedExpUpgradeTable = nullptr;

	/** レベル */
	int32 Level;

	/** 無敵状態かどうか */
	bool bIsInvincible;

	/** 行名 -> 現在の強化レベル（配列インデックス） */
	UPROPERTY()
	TMap<FName, int32> UpgradeLevels;

};
