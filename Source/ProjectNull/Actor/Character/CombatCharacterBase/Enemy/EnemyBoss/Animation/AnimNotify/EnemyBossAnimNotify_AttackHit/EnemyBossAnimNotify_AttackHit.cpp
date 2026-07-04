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
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBoss/EnemyBossBase.h>
#include <ProjectNull/System/Interface/DamageableInterface/DamageableInterface.h>
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

// ------------------------------------------------------------------------------------
// 当たり判定の開始処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_AttackHit::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	// 振り始め：ヒット済みリストをクリア
	HitActors.Empty();

	// 地面エフェクト用
	bGroundImpactEffect = false;
	ElapsedTime = 0.0f;
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
	const FTransform SocketTransform = MeshComp->GetSocketTransform(SocketName);
	// SphereOffsetをSocketの基準のローカルオフセットとして利用する
	const FVector HitCenter = SocketTransform.TransformPosition(SphereOffset);

	// Notifyからの経過時間を取得
	ElapsedTime += FrameDeltaTime;

	// 一度だけ地面エフェクトを出す
	if (ElapsedTime >= GroundImpactDelay && !bGroundImpactEffect && bSpawnGroundImpactEffect)
	{
		if(TrySpawnGroundImpactEffect(MeshComp, HitCenter, SocketTransform))
		{
			bGroundImpactEffect = true;
		}
	}

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

		// IDamageableInterfaceを継承しているオブジェクトを判定
		IDamageableInterface* DamageInterface = Cast<IDamageableInterface>(Target);
		if (!DamageInterface) { continue; }

		// この振りで当てていたActorはスキップ、ヒットしたらリストにActorを追加
		if (HitActors.Contains(Target)) { continue; }
		HitActors.Add(Target);

		int Damage = 10;

		DamageInterface->ReceiveDamage(Damage);
	}

	// デバッグ表示
	if (bDrawDebug)
	{
		DrawDebugSphere(World, HitCenter, Radius, 12, FColor::Red, false, 0.05f);
	}

	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(Owner);
	if (!IsValid(Boss)) { return; }

	Boss->TryConsumeFastFallRequest(); // 落下中の攻撃であれば、落下中フラグを消費する
}

// ------------------------------------------------------------------------------------
// 当たり判定の終了処理
// ------------------------------------------------------------------------------------
void UEnemyBossAnimNotify_AttackHit::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	AActor* Owner = MeshComp->GetOwner();			// メッシュの持ち主（Actor）を取得
	AEnemyBossBase* Boss = Cast<AEnemyBossBase>(Owner);
	if (!IsValid(Boss)) { return; }

	if (Boss != nullptr)
	{
		Boss->ResetGravity();
	}

	// 振り終わり：リストをクリア
	HitActors.Empty();
}

bool UEnemyBossAnimNotify_AttackHit::TrySpawnGroundImpactEffect(
	USkeletalMeshComponent* MeshComp, 
	const FVector&			TraceCenter,
	const FTransform&		SourceTransform)
{
	if (!IsValid(MeshComp)) 
	{ return false; }

	UWorld* World = MeshComp->GetWorld();
	AActor* Owner = MeshComp->GetOwner();

	if (!IsValid(World) || !IsValid(Owner)) { return false; }

	const FVector Start = TraceCenter + FVector(0.0f, 0.0f, GroundTraceUpDistance);
	const FVector End	= TraceCenter - FVector(0.0f, 0.0f, GroundTraceDownDistance);

	FHitResult Hit;

	FCollisionQueryParams Params(SCENE_QUERY_STAT(BossGroundImpactEffect), true);
	Params.AddIgnoredActor(Owner);

	// これがないと Physical Material が取れない
	Params.bReturnPhysicalMaterial = true;

	const bool bHit = World->LineTraceSingleByChannel(
		Hit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	if (bDrawGroundImpactDebug)
	{
		DrawDebugLine(
			World,
			Start,
			End,
			bHit ? FColor::Green : FColor::Red,
			false,
			2.0f,
			0,
			2.0f
		);

		if (bHit)
		{
			DrawDebugSphere(
				World,
				Hit.ImpactPoint,
				20.0f,
				12,
				FColor::Blue,
				false,
				2.0f
			);
		}
	}

	if (!bHit)
	{
		return false;
	}

	EPhysicalSurface SurfaceType = SurfaceType_Default;

	if (Hit.PhysMaterial.IsValid())
	{
		SurfaceType = UPhysicalMaterial::DetermineSurfaceType(Hit.PhysMaterial.Get());
	}

	UNiagaraSystem* Effect = GetGroundImpactEffect(SurfaceType);

	if (!Effect)
	{
		return false;
	}

	const FVector SpawnLocation = Hit.ImpactPoint + Hit.ImpactNormal * 2.0f;

	// エフェクトの向きとVelocityを作る
	const FVector SocketForward = SourceTransform.GetUnitAxis(EAxis::X);	// Xが前、Yが右、Zが上
	const FVector SocketRight	= SourceTransform.GetUnitAxis(EAxis::Y);
	const FVector SocketUp		= SourceTransform.GetUnitAxis(EAxis::Z);

	// NiagaraのZ方向を地面の法線に合わせる
	FVector EffectForward = FVector::VectorPlaneProject(SocketForward, Hit.ImpactNormal).GetSafeNormal();
	if (EffectForward.IsNearlyZero())
	{
		EffectForward = FVector::VectorPlaneProject(MeshComp->GetOwner()->GetActorForwardVector(), Hit.ImpactNormal).GetSafeNormal();
	}

	// X方向 = 攻撃Socketの前方向　Z方向 = 地面の法線方向
	const FRotator SpawnRotation = FRotationMatrix::MakeFromXZ(EffectForward, Hit.ImpactNormal).Rotator();

	// 攻撃方向に飛ぶ速度 + 地面から少し跳ねる速度
	const FVector ImpactVelocity = EffectForward * VelocityPower + Hit.ImpactNormal * UpVelocityPower;

	UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAtLocation(
		World,
		Effect,
		SpawnLocation,
		SpawnRotation,
		FVector(1.0f),
		false,
		false,
		ENCPoolMethod::AutoRelease,
		true
	);

	// Niagaraを作って、falseでいきなり動かさず、NiagaraVelocityをセットした後にActivateをtrueにする
	if (NiagaraComp)
	{
		NiagaraComp->SetNiagaraVariableVec3(
			VelocityParameterName.ToString(), ImpactVelocity);

		NiagaraComp->Activate(true);
	}

	UE_LOG(LogTemp, Warning, TEXT("Ground Impact Surface: %d / PhysMat: %s / HitActor: %s"),
		static_cast<int32>(SurfaceType),
		*GetNameSafe(Hit.PhysMaterial.Get()),
		*GetNameSafe(Hit.GetActor())
	);

	return true;
}

UNiagaraSystem* UEnemyBossAnimNotify_AttackHit::GetGroundImpactEffect(EPhysicalSurface SurfaceType) const
{
	if (const TObjectPtr<UNiagaraSystem>* FoundEffect = GroundImpactEffects.Find(SurfaceType))
	{
		return FoundEffect->Get();
	}

	return DefaultGroundImpactEffect.Get();
}
