#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossPlayAttack.generated.h"

class AEnemyBossBase;

/*
* 攻撃するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossPlayAttack : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossPlayAttack(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	/** 選択されている攻撃パターン*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	FBossAttackPattern CurrentAttack;

	/**	 プレイヤーの方向を向いたらtrueにして攻撃モンタージュを開始する*/
	bool bMontageStarted = false;

	/** 連撃時にプレイヤーの方向を向きながら攻撃するフラグ*/

};