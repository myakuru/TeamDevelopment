// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "EnemyBossAnimNotify_BreathHit.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include <ProjectNull/System/Interface/DamageableInterface/DamageableInterface.h>

// ------------------------------------------------------------------------------------
// 当たり判定の開始処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_BreathHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 振り始め：ヒット済みリストをクリア
	HitCooldownMap.Empty();

	AActor* Owner = MeshComp->GetOwner();
	// Niagaraコンポーネントを名前で取得
	if (AEnemyBossBase* Boss = Cast<AEnemyBossBase>(Owner))
	{
		BreathNiagara = Boss->GetBreathNiagara();

		if (UNiagaraComponent* NC = BreathNiagara.Get())
		{
			NC->Activate(true);
		}
	}
}

// ------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_BreathHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	// クールダウンを全Actorで進める
	for (auto& [Actor, Time] : HitCooldownMap)
	{
		Time -= FrameDeltaTime;
	}
	/*for (auto& Pair : HitCoolDownMap)
	{
		Pair.Time -= DeltaTime;
	}*/

	if (!IsValid(MeshComp)) { return; }

	AActor* Owner = MeshComp->GetOwner();			// メッシュの持ち主（Actor）を取得
	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(Owner);
	UWorld* World = MeshComp->GetWorld();			// メッシュのレベル空間のポインタを取得
	if (!IsValid(Owner) || !IsValid(World)) { return; }
	if (!IsValid(Boss)) {return;}

	const FVector Start = MeshComp->GetSocketLocation(SocketName);
	const FVector Forward = MeshComp->GetSocketRotation(SocketName).Vector();
	const FVector End = Start + Forward * Range;

	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	Owner->GetWorld()->LineTraceMultiByChannel(
		Hits, Start, End, ECC_Pawn, Params);

	// Niagaraに終点を渡す
	/*const FVector BeamEnd = Hits.IsEmpty() ? End : Hits[0].ImpactPoint;
	if (BreathNiagara.IsValid())
	{
		if (UNiagaraComponent* NC = BreathNiagara.Get())
		{
			NC->SetNiagaraVariableVec3(TEXT("BeamEnd"), BeamEnd);
		}
	}*/

	// ヒットした全アクターに個別にダメージを送る
	for (const FHitResult& Hit : Hits)
	{
		AActor* HitActor = Hit.GetActor();
		if (!IsValid(HitActor)) continue;

		// IDamageableInterface を実装していなければスキップ
		IDamageableInterface* Damageable = Cast<IDamageableInterface>(HitActor);
		if (!Damageable) continue;

		// クールダウン中はスキップ
		float& Cooldown = HitCooldownMap.FindOrAdd(HitActor, 0.f);
		if (Cooldown > 0.f) continue;

		int Damage = 10;

		Damageable->ReceiveDamage(Damage);
		Cooldown = HitInterval;
	}
}

// ------------------------------------------------------------------------------------
// 当たり判定の終了処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_BreathHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	HitCooldownMap.Empty();

	if (UNiagaraComponent* NC = BreathNiagara.Get())
	{
		NC->Deactivate();
	}

	BreathNiagara.Reset();
}
