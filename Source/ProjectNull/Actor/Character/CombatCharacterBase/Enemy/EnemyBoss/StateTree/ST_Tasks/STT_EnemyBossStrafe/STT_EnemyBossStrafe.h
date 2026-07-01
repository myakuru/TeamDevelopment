#pragma once

#include "CoreMinimal.h"
#include "StateTreeTaskBase.h"
#include "Blueprint/StateTreeTaskBlueprintBase.h"
#include "../../../EnemyBossDataStruct.h"
#include "../STT_EnemyBossTaskBase.h"
#include "STT_EnemyBossStrafe.generated.h"

class AEnemyBossBase;

/*
* 攻撃を選択するタスク
*/
UCLASS()
class PROJECTNULL_API USTT_EnemyBossStrafe : public USTT_EnemyBossTaskBase
{
	GENERATED_BODY()

public:

	// 進行方向
	struct NextVelocity
	{
		FVector Forward		= { 1.0f,0.0f,0.0f };	// 前
		FVector Backward	= { -1.0f,0.0f,0.0f	};	// 後ろ
		FVector Left		= { 0.0f,-1.0f,0.0f };	// 左
		FVector Right		= { 0.0f,1.0f,0.0f	};	// 右
	};

	USTT_EnemyBossStrafe(const FObjectInitializer& a_ObjInit);

	/** タスク系*/
	virtual EStateTreeRunStatus Tick(FStateTreeExecutionContext& Context, const float DeltaTime);
	virtual EStateTreeRunStatus EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition);
	virtual	void ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float MoveSpeed = 200.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float Elapsed = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float StrafeTime = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite,Category = "Input")
	float StrafeSpeed = 100.0f;

	//UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	float StrifeDir = 1.0f;

	/** 一定範囲内に入っていたら後ろに下がる*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float InsiteDistance = 400.0f;

	/** 一定範囲外にターゲットが出たらステートを抜ける用*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input")
	float GetOutDistance = 1200.0f;

	/** 後ろに動くフラグ*/
	/*UPROPERTY(EditAnywhere, Category = "Input")*/
	bool bBackStep = false;

	UPROPERTY(EditAnywhere, Category = "Input")
	FName StrafeLoopSectionName = TEXT("Loop");

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Orbit")
	float OrbitRadius = 700.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Input|Orbit")
	float OrbitRadiusCorrection = 0.4f;

	NextVelocity NextVel;
};