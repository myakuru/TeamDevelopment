#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h"
#include "STT_EnemyAnimation.generated.h"

class AEnemyBase;

/**
 * アニメーションを再生するタスク
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyAnimation : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()

public:

	USTT_EnemyAnimation(const FObjectInitializer& a_ObjInit);

public:

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)								override;

private:

	/**
	 * @brief 持ち主の敵クラス
	 */
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy;

	/**
	 * @brief 再生したいアニメーションのステート名
	 */
	UPROPERTY(EditAnywhere, Category = "Input")
	EEnemyState AnimationState = EEnemyState::Idle;

	/**
	 * @brief アニメーションの長さ
	 */
	UPROPERTY(EditAnywhere)
	float AnimationDuration = 1.f;

	/**
	 * @brief ブレンド速度
	 */
	UPROPERTY(EditAnywhere)
	float BlendSpeed = 0.2f;

	/**
	 * @brief ループするか
	 */
	UPROPERTY(EditAnywhere)
	bool IsLooping = false;

};
