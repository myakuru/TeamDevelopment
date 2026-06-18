#include "STT_EnemyDead.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/GameProgressSubsystem/GameProgressSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/System/WorldSystem/EnemyPoolSubSystem/EnemyPoolSubSystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyISMManager/EnemyISMManager.h>
#include <ProjectNull/Data/CharacterRuntimeData/EnemyRuntimeData/EnemyRuntimeData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

USTT_EnemyDead::USTT_EnemyDead(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyDead::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	// 前ステートの終了フラグをリセット
	OwnerEnemy->GetEnemyRuntimeData()->ResetAnimFinished();
	// 再生したいアニメを設定（インデックス・ループOFF・ブレンド開始）
	OwnerEnemy->PlayAnimation(static_cast<uint32>(EEnemyState::Death), false);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyDead::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Super::Tick(a_Context, a_DeltaTime);

	// アニメが1周したらSucceededを返してStateTreeに遷移を委ねる
	/*if (OwnerEnemy->GetEnemyRuntimeData()->GetAnimFinished())
	{
		return EStateTreeRunStatus::Succeeded;
	}*/

	return EStateTreeRunStatus::Succeeded;
}

void USTT_EnemyDead::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	if (!OwnerEnemy) { return; }

	Super::ExitState(a_Context, a_Transition);

	// 死亡処理の実行
	OwnerEnemy->FinalizeDeath();
}
