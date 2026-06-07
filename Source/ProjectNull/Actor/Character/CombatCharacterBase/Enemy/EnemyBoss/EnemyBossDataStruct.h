#pragma once

#include "EnemyBossDataStruct.generated.h"

class UAnimMontage;

USTRUCT(BlueprintType)
struct FBossAttackPattern
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinRange = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxRange = 300.0f;

	/** trueなら最後まで出し切る（確定連撃） */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bConfirmedCombo = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TObjectPtr<UAnimMontage>> AttackMontages;
};

UENUM(BlueprintType)
enum class EBossActionType : uint8
{
	None,
	PlayAttack,        // 近接攻撃を出す
	Strafe,        // プレイヤーの周りを回って様子見
	ApproachWalk,  // 歩いて近づく
	ApproachRun,   // 走って近づく
	JumpAttack,    // 跳びかかる
	RangedAttack   // 遠距離攻撃
};
