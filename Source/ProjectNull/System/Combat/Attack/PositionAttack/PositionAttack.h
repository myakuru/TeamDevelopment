#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/System/Combat/Attack/AttackBase.h"
#include "PositionAttack.generated.h"

/** 前方宣言 */
class USphereAttack;

/**
 * 指定座標に攻撃を行うクラス
 */
UCLASS()
class PROJECTNULL_API UPositionAttack : public UAttackBase
{
	GENERATED_BODY()
	
public:
	
	UPositionAttack();
	
public:
	
	/**
	 * @brief 初期化処理
	 * @param InOwner オーナーアクター
	 */
	virtual void Initialize(const TObjectPtr<AActor>& InOwner)override;

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute(const FVector& InTargetLocation = FVector::ZeroVector) override;

	/**
	 * @brief 中止処理
	 */
	virtual void Cancel()override;

	/**
	 * @brief 更新処理
	 * @param InDeltaTime デルタタイム
	 */
	virtual void Update(float InDeltaTime)override;
	
public:
	
	/**
	 * @brief 球状の攻撃処理を行うサブクラス
	 */
	UPROPERTY(EditAnywhere)
	TSubclassOf<USphereAttack> SubSphereAttack;
	
	/**
	 * @brief 球状の攻撃処理を行うクラス
	 */
	UPROPERTY()
	TObjectPtr<USphereAttack> SphereAttack;
	
};
