#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyDataStruct.h"
#include <ProjectNull/Utility/StateMachine/StateDataBase.h>
#include "EnemyDataAsset.generated.h"

UCLASS(BlueprintType)
class PROJECTNULL_API UEnemyDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:

	// 移動方向
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	// 移動速度
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	// 回転補間速度
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;

	// スケーリング計算用ヒットポイント
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	// スケーリング計算用攻撃力
	UPROPERTY(EditAnywhere)
	FStatScaling AttackScaling;

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