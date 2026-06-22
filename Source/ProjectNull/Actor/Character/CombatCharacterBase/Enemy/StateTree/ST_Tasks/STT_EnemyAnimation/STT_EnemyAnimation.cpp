#include "STT_EnemyAnimation.h"
#include "StateTreeExecutionContext.h"

#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\Data\CharacterRuntimeData\EnemyRuntimeData\EnemyRuntimeData.h>

USTT_EnemyAnimation::USTT_EnemyAnimation(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	OwnerEnemy(nullptr)
	,	AnimationState(EEnemyState::Idle)
	,	AnimationDuration(1.f)
	,	IsLooping(false)
{
	// Tick有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyAnimation::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	if (auto EnemyRuntime = OwnerEnemy->GetEnemyRuntimeData())
	{
		EnemyRuntime->ResetAnimFinished();		// 前ステートの終了フラグをリセット
		EnemyRuntime->StartAnimMonitor(
			static_cast<int32>(AnimationState),	// 再生したいアニメーションステート名
			IsLooping,							// ループするか
			AnimationDuration					// アニメーションの時間
		);
	
		// 再生したいアニメを設定（インデックス・ループOFF・ブレンド開始）
		OwnerEnemy->PlayAnimation(static_cast<int32>(AnimationState), IsLooping, BlendSpeed);
	}

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyAnimation::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { EStateTreeRunStatus::Failed; }
	
	if (auto EnemyRuntime = OwnerEnemy->GetEnemyRuntimeData())
	{
		// CPU側のアニメーション情報を更新
		EnemyRuntime->UpdateAnimationMonitor(a_DeltaTime);
	}

	return EStateTreeRunStatus::Running;
}
