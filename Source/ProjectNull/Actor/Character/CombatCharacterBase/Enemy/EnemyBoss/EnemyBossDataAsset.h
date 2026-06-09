#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossDataAsset.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyBossAttackSet : public UPrimaryDataAsset
{
	GENERATED_BODY()
public:
	/** このボスが使う攻撃パターン一覧 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Attack")
	TArray<FBossAttackPattern> Patterns;

	// 移動方向
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	// 回転補間速度
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;

	// 最終的なヒットポイント
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	// 最終的な攻撃力
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

	// エネミーの重量
	UPROPERTY(EditAnywhere)
	float	KnockBackWeight = 1.0f;

	// ノックバック方向
	FVector KNockBackVelocity = FVector::ZeroVector;

	// エネミーが吹き飛び中の判定フラグ
	bool	KnockBackFlg = false;

	// 経験値
	UPROPERTY(EditAnywhere)
	float Exp = 0;

	// ギアエネルギー
	UPROPERTY(EditAnywhere)
	float GearEnergy = 0;

	// プレイヤーとの距離
	float DistancePlayer = 0.0f;

	// 攻撃可能距離
	UPROPERTY(EditAnywhere)
	float AttackDistance = 20.0f;

	// 攻撃可能フラグ
	bool CanAttack = false;

	/** 生存フラグ*/
	UPROPERTY(EditAnywhere)
	bool IsAlive = true;
};
