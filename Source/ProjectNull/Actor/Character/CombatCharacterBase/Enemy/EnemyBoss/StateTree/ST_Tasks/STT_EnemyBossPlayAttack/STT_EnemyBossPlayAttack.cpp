#include "STT_EnemyBossPlayAttack.h"

#include "GameFramework/Actor.h"
#include "StateTreeExecutionContext.h"
#include "NavigationSystem.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USTT_EnemyBossPlayAttack::USTT_EnemyBossPlayAttack(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
{
	// Tick処理有効化
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyBossPlayAttack::Tick(FStateTreeExecutionContext& Context, const float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayAttack Tick In"));

	// どれかポインタがなければリターン
	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	// オーナーのボスチェック
	AEnemyBossBase* Boss = GetBoss();
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// メッシュチェック
	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim)) { return EStateTreeRunStatus::Failed; }

	// まず向き直る、プレイヤーに向きなおったらMontage再生
	if (!bMontageStarted)
	{
		FVector Dir = TargetActor->GetActorLocation() - Boss->GetActorLocation();
		Dir.Z = 0.0f;
		Dir.Normalize();

		const FRotator Want(0.0f, Dir.Rotation().Yaw, 0.0f);
		const FRotator Now = Boss->GetActorRotation();
		Boss->SetActorRotation(FMath::RInterpTo(Now, Want, DeltaTime, 10.0f));

		// 十分プレイヤーを向けたら攻撃開始
		const float YawDiff = FMath::Abs(FMath::FindDeltaAngleDegrees(Now.Yaw, Want.Yaw));
		if (YawDiff <= 10.0f)   // 10度以内まで向いたらOK
		{
			const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
			if (Atk.AttackMontages.Num() == 0) { return EStateTreeRunStatus::Failed; }
			Anim->Montage_Play(Atk.AttackMontages[Boss->GetHitIndex()]);
			bMontageStarted = true;
		}
		return EStateTreeRunStatus::Running;   // 向き直り中
	}

	// Tick：今のモンタージュが終わったら次へ
	const FBossAttackPattern& Atk = Boss->GetCurrentAttack();
	// 指定した配列内にアニメーションがあるかチェック
	if (!Atk.AttackMontages.IsValidIndex(Boss->GetHitIndex()))
	{
		UE_LOG(LogTemp, Warning, TEXT("PlayAttack Tick AnimEnd"));
		return EStateTreeRunStatus::Succeeded;
	}
	// 現在のアニメーションモンタージュを取得
	UAnimMontage* Cur = Atk.AttackMontages[Boss->GetHitIndex()];
	// アニメーションモンタージュの終了チェック
	if (!Anim->Montage_IsPlaying(Cur))
	{
		Boss->AdvanceHitIndex();
		// 次のアニメーションがあり、flgがtrueなら
		// 上述の処理に入り直して次のアニメーションを実行
		if (Atk.AttackMontages.IsValidIndex(Boss->GetHitIndex()))
		{
			bMontageStarted = false;
			return EStateTreeRunStatus::Running;
		}
		return EStateTreeRunStatus::Succeeded;
	}

	UE_LOG(LogTemp, Warning, TEXT("PlayAttack Tick Out"));

	return EStateTreeRunStatus::Running;

	//// モンタージュが終わったら成功で抜ける → 遷移でSelectへ戻る
	//if (!Anim->Montage_IsPlaying(CurrentAttack.AttackMontage))
	//{
	//	UE_LOG(LogTemp, Warning, TEXT("MontageEnd"));
	//	return EStateTreeRunStatus::Succeeded;
	//}

	//return EStateTreeRunStatus::Running;  // 再生中は待つ
}

// タスク開始時の処理
EStateTreeRunStatus USTT_EnemyBossPlayAttack::EnterState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	UE_LOG(LogTemp, Warning, TEXT("PlayAttack EnterState In"));

	Super::EnterState(Context, Transition);

	if (!HasValidRefs()) { return EStateTreeRunStatus::Failed; }

	// ボスを確認して、選択中の攻撃を受け取る
	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(OwnerBoss);
	if (!IsValid(Boss)) { return EStateTreeRunStatus::Failed; }

	// AnimInstanceを取得して再生
	USkeletalMeshComponent* Mesh = Boss->GetMesh();
	UAnimInstance* Anim = Mesh ? Mesh->GetAnimInstance() : nullptr;
	if (!IsValid(Anim))
	{
		return EStateTreeRunStatus::Failed;
	}

	bMontageStarted = false;

	// 攻撃の瞬間に移動を止める（残った移動でクルッと回るのを防ぐ）
	AIC->StopMovement();

	Boss->ResetHitIndex();

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyBossPlayAttack::ExitState(FStateTreeExecutionContext& Context, const FStateTreeTransitionResult& Transition)
{
	Super::ExitState(Context, Transition);
}
