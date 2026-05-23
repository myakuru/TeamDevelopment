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
#include <ProjectNull/GameInstance/SuperGameInstance.h>
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

	//// 死亡アニメーションの再生やエフェクトの発生を行う
	OwnerEnemy->SetEnemyState(EEnemyState::Dead);
	// 敵が死んだ際に敵管理クラス経由でリストから自身を削除する
	/*if (auto EnemyManager = OwnerEnemy->GetEnemyManagerSubsystem()) {
		EnemyManager->RemoveEnemy(OwnerEnemy);
		if (auto* ISMManager = EnemyManager->GetISMManager(OwnerEnemy->GetISMManagerClass()))
		{
			ISMManager->UnregisterEnemy(OwnerEnemy);
		}
	}*/

	// 敵が死んだ際にゲームの進行管理クラス経由で倒した敵数を加算する
	if (auto GameProgress = OwnerEnemy->GetGameProgressSubsystem()) {
		GameProgress->AddKillCount();
	}

	//OwnerEnemy->SpawnDeathEffect();

	//OwnerEnemy->SpawnDeathExperience();

	OwnerEnemy->OnDeath();

	// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	/*if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		GameInstance->GetPlayerRuntimeData()->AddExperience(EnemyStatus.Exp);
		GameInstance->GetPlayerRuntimeData()->AddGearEnergy(EnemyStatus.GearEnergy);
	}*/

	// PoolSubSystemに返却する
	// Return()の中でDeactivate()が呼ばれて非表示・Tick停止でPool待機に戻る
	if (UEnemyPoolSubSystem* PoolSubSystem =
		GetWorld()->GetSubsystem<UEnemyPoolSubSystem>())
	{
		PoolSubSystem->Return(OwnerEnemy);
	}

	return EStateTreeRunStatus();
}

EStateTreeRunStatus USTT_EnemyDead::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }
	return EStateTreeRunStatus::Running;
}

void USTT_EnemyDead::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}
