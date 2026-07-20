#pragma once

#include "CoreMinimal.h"
#include "../CollisionAttack.h"
#include "BoxAttack.generated.h"

/* ボックス状の当たり判定を持つアクタークラス */
class ABoxCollision;

/**
 * ボックス状の攻撃処理を行うクラス
 */
UCLASS()
class PROJECTNULL_API UBoxAttack : public UCollisionAttack
{
	GENERATED_BODY()
	
public:

	UBoxAttack();

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

private:

	/**
	 * @brief デフォルトエディタ側でされるようにする
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ABoxCollision> SubBoxCollision;

	/**
	 * @brief ボックス状の当たり判定アクター
	 */
	UPROPERTY()
	TObjectPtr<ABoxCollision> BoxCollision;
};
