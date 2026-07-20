#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossJumpAttack.generated.h"

class AEnemyBossBase;

UENUM()
enum class EBossJumpPhase : uint8 { Takeoff, WaitLiftOff, Air, Land };

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossJumpAttack : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	USTT_EnemyBossJumpAttack(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	///** 跳躍の水平速度 */
	//UPROPERTY(EditAnywhere, Category = "Parameter")
	//float LaunchHorizontal = 900.0f;

	///** 跳躍の垂直速度 */
	//UPROPERTY(EditAnywhere, Category = "Parameter")
	//float LaunchVertical = 600.0f;

	/** 跳躍アークの高さ（0=直線的, 1=高く）*/
	UPROPERTY(EditAnywhere, Category = "Input")
	float ArcParam = 0.5f;

	/**プレイヤーの方向を向いたらtrueにして攻撃モンタージュを開始する*/
	bool bMontageStarted = false;

private:
	EBossJumpPhase Phase = EBossJumpPhase::Takeoff;
	bool bLeftGround		= false;   // 一度地面を離れたか（着地誤検出防止）
	bool bLandMontagePlayed = false;

};
