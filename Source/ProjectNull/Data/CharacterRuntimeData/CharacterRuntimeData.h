
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "CharacterRuntimeData.generated.h"


/** 攻撃力関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FAttackRuntimeData
{
	GENERATED_BODY()

public:

	FAttackRuntimeData() :
		Final(0.0f)
	{
	}

	/** 最終的な攻撃力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	float Final;
};

/** 体力関連Runtimeデータ構造体 */
USTRUCT(BlueprintType)
struct FHealthRuntimeData
{
	GENERATED_BODY()

public:

	FHealthRuntimeData()
		: Current	(0.0f)
		, Max		(0.0f)
	{
	}

	void SetCurrent(float NewCurrent)
	{
		Current = FMath::Clamp(NewCurrent, 0.0f, Max);
	}

	/** 現在の体力 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Current;

	/** 最大体力 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float Max;
};

/**
  キャラクターのRuntimeデータ中間基底クラス
 */
UCLASS()
class PROJECTNULL_API UCharacterRuntimeData : public UObject
{
	GENERATED_BODY()

public:
	UCharacterRuntimeData();

public:
	virtual void Initialize() { return; }

	/** ゲッター */
	/** 体力を取得する処理 */
	float GetHealth() const { return Health.Current; }

	/** 体力の最大値を取得 */
	float GetMaxHealth() const { return Health.Max; }

	/** セッター */
	/** 体力を固定値にセットする処理 */
	void SetHealth(float inCurrentHealth) { Health.SetCurrent(inCurrentHealth); }

	/** 体力を加算する処理 */
	void AddHealth(float Amount) { SetHealth(Health.Current + Amount); }

protected:

	/** 体力関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Health")
	FHealthRuntimeData Health;

	/** 攻撃力関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	FAttackRuntimeData Attack;
};
