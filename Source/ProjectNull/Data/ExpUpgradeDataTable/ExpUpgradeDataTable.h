// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "ExpUpgradeDataTable.generated.h"

/**
 * 強化効果の種別。
 * 効果を増やすときは、ここに列挙子を1つ追加し、
 * その効果を実際に使う場所（ステータス計算）で参照するだけでよい。
 * 適用処理側（PlayerRuntimeData）に Id ごとの分岐を足す必要はない。
 */
UENUM(BlueprintType)
enum class EUpgradeEffectType : uint8
{
	/** 攻撃力の倍率 */
	AttackDamage	UMETA(DisplayName = "攻撃力"),
	PlayerSpeed		UMETA(DisplayName = "プレイヤーの移動速度"),
};

/** 1レベル分の強化情報 */
USTRUCT(BlueprintType)
struct FExpUpgradeLevelData
{
	GENERATED_BODY()

	/** 説明文 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	/** この強化がどのステータスに作用するか */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EUpgradeEffectType EffectType = EUpgradeEffectType::AttackDamage;

	/** EffectType に対する効果量（倍率） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AttackMultiplier = 1.0f;
};


/**
 * 強化画面のテキストとIdを管理するデーターテーブル
 */
USTRUCT(BlueprintType)
struct PROJECTNULL_API FExpUpgradeRow : public FTableRowBase
{
	GENERATED_BODY()

	/** 攻撃倍率 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FExpUpgradeLevelData> UpgradeLevels;
};

/**
 * 強化画面のUIに提示する1件分の候補情報。
 * ドメイン（PlayerRuntimeData）が生成し、UIは表示に使うだけ。
 */
USTRUCT(BlueprintType)
struct FValidUpgradeInfo
{
	GENERATED_BODY()

	/** DataTable の行名（＝強化ID） */
	FName RowName;

	/** 表示する説明文 */
	FText Description;

	/** 提示時点での強化レベル */
	FName CurrentLevel = "0";
};
