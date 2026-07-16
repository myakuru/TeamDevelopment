// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CollisionAttack.h"
#include "SphereAttack.generated.h"

/** 球状の当たり判定を持つアクタークラス */
class ASphereCollision;

/** 攻撃の警告を出力するアクタークラス */
class AWarningShapeActor;

/**
 * 球状の攻撃処理を行うクラス
 */
UCLASS()
class PROJECTNULL_API USphereAttack : public UCollisionAttack
{
	GENERATED_BODY()
	
public:
	
	USphereAttack();

public:

	/**
	 * @brief 初期化処理
	 * @param Owner オーナーアクター
	 */
	virtual void Initialize(const TObjectPtr<AActor>& InOwner)override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute(const FVector& InTargetLocation = FVector::ZeroVector)override;

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel()override;

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float InDeltaTime)override;

	/* ~Begin Setters */
	/**
	 * @brief 球コリジョンのトランスフォームに適応(攻撃判定にも親子関係で適応される)
	 */
	virtual void ApplyCollisionTransform(const FTransform& InTransform)override;
	
	
	
	/* End Setters~ */
	
private:

	/**
	 * @brief デフォルトエディタ側でされるようにする
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASphereCollision> SubSphereCollision;

	/**
	 * @brief 球状の当たり判定アクター
	 */
	UPROPERTY()
	TObjectPtr<ASphereCollision> SphereCollision;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AWarningShapeActor> WarningShapeActor;
	
	/**
	 * @brief 攻撃の警告を知らせるアクタークラス配列
	 */
	UPROPERTY()
	TArray<TObjectPtr<AWarningShapeActor>> WarningShapes;
	
	/**
	 * @brief 警告を出すか
	 */
	UPROPERTY(EditAnywhere)
	bool IsShowWarning=false;
	
};
