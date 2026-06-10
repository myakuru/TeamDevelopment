// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "EnemyBossAnimNotify_AttackHit.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/OverlapResult.h"
#include "DrawDebugHelpers.h"

// ------------------------------------------------------------------------------------
// 当たり判定の開始処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_AttackHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 振り始め：ヒット済みリストをクリア
	HitActors.Empty();
}

// ------------------------------------------------------------------------------------
// 当たり判定処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_AttackHit::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime, EventReference);

	if (!IsValid(MeshComp)) { return; }

	AActor* Owner = MeshComp->GetOwner();			// メッシュの持ち主（Actor）を取得
	UWorld* World = MeshComp->GetWorld();			// メッシュのレベル空間のポインタを取得
	if (!IsValid(Owner) || !IsValid(World)) { return; }

	// ソケットの現在位置を取得 → ここに当たり判定を出す
	const FVector HitCenter = MeshComp->GetSocketLocation(SocketName);

	// スフィアで重なり判定
	TArray<FOverlapResult> Overlaps;
	FCollisionShape Sphere = FCollisionShape::MakeSphere(Radius);

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(Owner);

	// 特定のチャネルを使用して、指定された位置でシェイプの衝突をテストし、オーバーラップするコンポーネントのセットを決定します。
	/*bool OverlapMultiByChannel(
	 TArray<struct FOverlapResult>&OutOverlaps, 
	 const FVector & Pos, const FQuat & Rot, 
	 ECollisionChannel TraceChannel, 
	 const FCollisionShape & CollisionShape, 
	 const FCollisionQueryParams & Params = FCollisionQueryParams::DefaultQueryParam, 
	 const FCollisionResponseParams & ResponseParam = FCollisionResponseParams::DefaultResponseParam
	 ) const;*/
	World->OverlapMultiByChannel(Overlaps, HitCenter, FQuat::Identity, ECC_Pawn, Sphere, Params);

	// OverlapMultiByChannelで衝突したオブジェクトのリストに当たり判定後の処理を行う
	for (const FOverlapResult & Overlap : Overlaps)
	{
		AActor* Target = Overlap.GetActor();
		if (!IsValid(Target)) { continue; }

		// この振りで当てていたActorはスキップ、ヒットしたらリストにActorを追加
		if (HitActors.Contains(Target)) { continue; }
		HitActors.Add(Target);

		int Damage = 10;

		AController* Inst = Owner->GetInstigatorController();
		UGameplayStatics::ApplyDamage(Target, Damage, Inst, Owner, nullptr);
	}

	// デバッグ表示
	if (bDrawDebug)
	{
		DrawDebugSphere(World, HitCenter, Radius, 12, FColor::Red, false, 0.05f);
	}
}

// ------------------------------------------------------------------------------------
// 当たり判定の終了処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_AttackHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	// 振り終わり：リストをクリア
	HitActors.Empty();
}
