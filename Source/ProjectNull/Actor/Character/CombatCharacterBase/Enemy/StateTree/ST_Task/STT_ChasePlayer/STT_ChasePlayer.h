#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "STT_ChasePlayer.generated.h"

/**
 * 持ち主クラス
 */
class AEnemyBase;

/**
 * プレイヤーへの追尾
 */
UCLASS()
class PROJECTNULL_API USTT_ChasePlayer : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	
	USTT_ChasePlayer(const FObjectInitializer& a_ObjInit)
		: Super(a_ObjInit)
	{
		/*
		* Tick処理有効化
		*/
		bShouldCallTick = true;
	}

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)					override;

private:

	void MoveToPlayer();

	/**
	 * @brief 持ち主のアドレス
	 */
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;

	/**
	 * @brief プレイヤーへの移動ベクトル
	 */
	UPROPERTY(EditAnywhere,Category="Input", meta = (ToolTip = "プレイヤーへの移動ベクトル"))
	FVector MoveDir = FVector::ZeroVector;
};