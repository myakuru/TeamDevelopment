#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyAttackData/EnemyAttackDataAsset.h"
#include "STT_EnemyMidBossAttack.generated.h"

/** 中ボスクラス */
class AEnemyMidBossBase;

/**
 * 中ボスの攻撃ステートクラス
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyMidBossAttack : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:
	
	USTT_EnemyMidBossAttack(const FObjectInitializer& InObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)	override;
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& InContext, const float InDeltaTime)								override;
	virtual void ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)					override;

private:

	/**	持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyMidBossBase> OwnerEnemy = nullptr;

	/**	発動する攻撃データ */
	UPROPERTY(EditAnywhere, Category = "Input")
	FEnemyAttackData AttackData;

};