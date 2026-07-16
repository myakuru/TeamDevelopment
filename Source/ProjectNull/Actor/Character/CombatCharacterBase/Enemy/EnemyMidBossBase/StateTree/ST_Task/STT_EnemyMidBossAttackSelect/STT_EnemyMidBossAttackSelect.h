#pragma once

#include "CoreMinimal.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyAttackData/EnemyAttackDataAsset.h"
#include "STT_EnemyMidBossAttackSelect.generated.h"

/** 中ボスのクラス */
class AEnemyMidBossBase;

/**
 * 中ボスの攻撃選択ステート
 * 距離で打ち分け
 */
UCLASS()
class PROJECTNULL_API USTT_EnemyMidBossAttackSelect : public UStateTreeTaskBlueprintBase
{
	GENERATED_BODY()
	
public:

	USTT_EnemyMidBossAttackSelect(const FObjectInitializer& InObjInit);

	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)	override;
	virtual void ExitState(FStateTreeExecutionContext& InContext, const FStateTreeTransitionResult& InTransition)					override;

private:

	/**	持ち主のアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyMidBossBase> OwnerEnemy = nullptr;

	/**	ターゲットとの距離の二乗値 */
	UPROPERTY(EditAnywhere, Category = "Input")
	float TargetDistanceSqr = 0.f;

	/**	攻撃の種類をキーとした配列を持つデータアセット */
	UPROPERTY(EditAnywhere,Category="Parameter")
	TObjectPtr<UEnemyAttackDataAsset> AttackDataAsset;
};