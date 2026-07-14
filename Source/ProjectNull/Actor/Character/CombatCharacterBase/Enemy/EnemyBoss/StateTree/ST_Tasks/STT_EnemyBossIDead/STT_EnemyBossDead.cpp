#include "STT_EnemyBossDead.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "GameFramework/Actor.h"

USTT_EnemyBossDead::USTT_EnemyBossDead(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossDead::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::EnterState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	if (IsValid(AIC))
	{
		AIC->StopMovement();
	}

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	Anim->Montage_Play(DeathMontage,1.0f);

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyBossDead::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	// まだ死亡アニメーションが終わっていない場合
	if (!bDeathAnimationEnd)
	{
		const float CurrentPosition = Anim->Montage_GetPosition(DeathMontage);
		const float MontageLength = DeathMontage->GetPlayLength();

		// 少し手前で止める
		// 完全な末尾だとブレンドや終了処理に入ることがあるため
		const float StopPosition = FMath::Max(0.0f, MontageLength - 0.33f);

		if (CurrentPosition >= StopPosition)
		{
			bDeathAnimationEnd = true;

			// 最終ポーズ付近に固定
			Anim->Montage_SetPosition(DeathMontage, StopPosition);
			Anim->Montage_Pause(DeathMontage);
		}
	}

	if (bDeathAnimationEnd)
	{
		DeathCount += DeltaTime;

		if (!bDeathEffectFlg)
		{
			if (DeathCount >= DeathEffectStart)
			{
				bDeathEffectFlg = true;
				Boss->SpawnDeathEffect();
				Boss->SetDeathMaterialChange();
			}
		}

		if (DeathCount >= DeathEffectDuration)
		{
			return EStateTreeRunStatus::Succeeded;
		}
	}


	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossDead::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return; }

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return; }

	Boss->BossFinalize();
	
}

