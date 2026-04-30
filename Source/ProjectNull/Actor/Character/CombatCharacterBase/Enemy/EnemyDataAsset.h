#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include <ProjectNull/Actor/Character/StateData/StateDataBase.h>
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

	// 最終的なヒットポイント
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	// スケーリング計算用ヒットポイント
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	// 最終的な攻撃力
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

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
	int EXP = 0;

	// ギアエネルギー
	UPROPERTY(EditAnywhere)
	int GearEnergy = 0;

	// プレイヤーとの距離
	float DistancePlayer = 0.0f;

	// 攻撃可能距離
	UPROPERTY(EditAnywhere)
	float AttackDistance = 20.0f;

	// 攻撃可能フラグ
	bool CanAttack = false;

	/** ステートごとのデータを管理するマップ*/
	UPROPERTY(EditAnywhere,BlueprintReadWrite,Instanced,Category = "StateData")
	TArray<TObjectPtr<UStateDataBase>> StateDataList;

	/** Stateデータをクラス型で判定する*/
	template<typename T>
	const T* GetStateData() const
	{
		for (const TObjectPtr<UStateDataBase>& Data : StateDataList)
		{
			if (const T* Casted = Cast<T>(Data.Get()))
			{
				return Casted;
			}
		}
		return nullptr;
	}
};