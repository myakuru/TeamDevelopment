
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacterBase.generated.h"

/// <summary>
/// ヒットポイントのデータ構造
/// </summary>
USTRUCT(BlueprintType)
struct FHitPointData
{
	GENERATED_BODY()

public:
	
	FHitPointData():
		Max(100.0f),
		Current(100.0f)
	{
	}

	// 最大HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float Max;

	// 現在のHP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float Current;
};

/// <summary>
/// 戦闘キャラクターの統計情報
/// </summary>
USTRUCT(BlueprintType)
struct FCombatCharacterStatistics
{
	GENERATED_BODY()

public:

	FCombatCharacterStatistics()
	{
	}

	// ヒットポイントのデータ
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	FHitPointData HP;
};

UCLASS()
class PROJECTNULL_API ACombatCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacterBase();

protected:

	virtual void BeginPlay() override;

	// 戦闘キャラクターの統計情報
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FCombatCharacterStatistics CombatStats;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/// <summary>
	/// 自身がダメージを受ける処理
	/// </summary>
	/// <param name="Damage">受けるダメージ量</param>
	virtual void ApplyDamage(float Damage = 1.0f);
	
};
