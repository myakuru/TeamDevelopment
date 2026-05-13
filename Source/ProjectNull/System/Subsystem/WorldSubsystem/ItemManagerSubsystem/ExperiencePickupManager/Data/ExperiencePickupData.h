/**
 * 経験値オーブに関するデータ構造
 */

#pragma once

#include "CoreMinimal.h"
#include "ExperiencePickupData.generated.h"

 /** オーブ1個の状態 */
USTRUCT(BlueprintType)
struct FExperiencePickupData
{
	GENERATED_BODY()

	/** 現在位置（Niagara描画位置・毎フレーム更新） */
	UPROPERTY()
	FVector Location = FVector::ZeroVector;

	/** 自然消滅するまでの残り時間（秒） */
	UPROPERTY()
	float LifeTime = 30.0f;

	/** プレイヤー取得時に加算される経験値量 */
	UPROPERTY()
	float ExpValue = 1.0f;

	/** Niagara描画色 */
	UPROPERTY()
	FLinearColor Color = FLinearColor::Green;

	/** Niagaraパーティクルのサイズ倍率 */
	UPROPERTY()
	float Size = 1.0f;

	/** 吸引を開始するプレイヤーとの距離 */
	UPROPERTY()
	float DetectRange = 1000.0f;

	/** 取得が確定するプレイヤーとの距離 */
	UPROPERTY()
	float PickupRange = 100.0f;

	/** 吸引中の移動速度（cm/s） */
	UPROPERTY()
	float ChaseSpeed = 100.0f;

	/** 現在プレイヤーを追尾しているか */
	UPROPERTY()
	bool bChasing = false;

	/** ターゲットの座標*/
	FVector ChaseStartPos;
	/** 追尾のふくらみ*/
	float RandomBulgeWidth;
	float RandomBulgeHeight;
	/** 追尾後の経過時間*/
	float ChaseElapsedTime = 0.0f;
};

/** BPのClassDefaultsで編集するデフォルト設定値 */
USTRUCT(BlueprintType)
struct FExperiencePickupSettings
{
	GENERATED_BODY()

	/** 吸引を開始するプレイヤーとの距離 */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float DetectRange = 500.0f;

	/** 取得が確定するプレイヤーとの距離 */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float PickupRange = 100.0f;

	/** 吸引中の移動速度（cm/s） */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float ChaseSpeed = 1000.0f;

	/** オーブが自然消滅するまでの時間（秒） */
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float LifeTime = 30.0f;

	/** オーブが追尾し始めて追いつくまでにかかる時間*/
	UPROPERTY(EditDefaultsOnly, Category = "Pickup")
	float ChaseDurationTime = 1.0f;
};