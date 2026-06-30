#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossBreath.generated.h"

class AEnemyBossBase;

UENUM()
enum class EBossBreathPhase : uint8 { Start, Loop, End };

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossBreath : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossBreath(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	/** ブレスの継続時間*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float BreathDuration	= 5.0f;

	/** ループ終了後のブレンド時間*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float BreathEndBlend	= 0.25f;

	UPROPERTY(EditAnywhere, Category = "Input")
	float BreathCount		= 0.0f;

	UPROPERTY(EditAnywhere, Category = "Input")
	FName BreathLoopSectionName = TEXT("Loop");

	/**プレイヤーの方向を向いたらtrueにして攻撃モンタージュを開始する*/
	bool bMontageStarted = false;

	UPROPERTY(Transient)
	EBossBreathPhase Phase = EBossBreathPhase::Start;

};