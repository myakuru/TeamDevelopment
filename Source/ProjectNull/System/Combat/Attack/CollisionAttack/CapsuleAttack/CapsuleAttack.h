// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CollisionAttack.h"
#include "CapsuleAttack.generated.h"

/* カプセル状の当たり判定を持つアクタークラス */
class ACapsuleCollision;

/**
 * カプセル状の攻撃処理を行うクラス
 */
UCLASS()
class PROJECTNULL_API UCapsuleAttack : public UCollisionAttack
{
	GENERATED_BODY()
	
public:
	
	UCapsuleAttack();

public:

	/**
	* @brief 初期化処理
	* @param Owner オーナーアクター
	*/
	virtual void Initialize(const TObjectPtr<AActor>& InOwner)override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute()override;

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel()override;

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float InDeltaTime)override;

private:
	
	/**
	 * @brief デフォルトエディタ側でされるようにする
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACapsuleCollision> SubCapsuleCollision;

	/**
	 * @brief カプセル状の当たり判定アクター
	 */
	UPROPERTY()
	TObjectPtr<ACapsuleCollision> CapsuleCollision;
};