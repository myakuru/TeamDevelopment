#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_EnemyMidBossWalk.generated.h"

class AEnemyMidBossBase;

/**
 * 中ボスの移動ステート
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyMidBossWalk : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTT_EnemyMidBossWalk(const FObjectInitializer& InObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)								override;

private:

	/**
	 * @brief 移動に必要なパラメータの初期化
	 */
	void InitializeWalkParams();

	/**
	 * @brief 移動処理
	 * @param a_DeltaTime デルタタイム
	 */
	void Move(const float InDeltaTime);

	/**
	 * @brief 移動ベクトルによって回転する処理
	 * @param InDelataTime デルタタイム
	 */
	void RotationToMoveDir(const float InDeltaTime);

private:

	/** 持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyMidBossBase> OwnerEnemy = nullptr;

	/** 移動ベクトル */
	UPROPERTY(EditAnywhere, Category = "Input")
	FVector MoveDir = FVector::ZeroVector;

	/**	回転速度 */
	UPROPERTY()
	float RotationInterpSpeed = 5.f;

};
