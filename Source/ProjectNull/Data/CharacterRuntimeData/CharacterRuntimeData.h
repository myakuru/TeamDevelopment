
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

	FHealthRuntimeData() :
		Max(100.0f),
		Current(100.0f)
	{
	}

	/** 最大体力 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Max;

	/** 現在の体力 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Health")
	float Current;
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

protected:

	/** 体力関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Health")
	FHealthRuntimeData Health;

	/** 攻撃力関連Runtimeデータ構造体 */
	UPROPERTY(EditAnywhere, Category = "Attack")
	FAttackRuntimeData Attack;
};
