#include "SphereAttack.h"

#include <ProjectNull/Actor/CollisionActor/SphereCollision/SphereCollision.h>

USphereAttack::USphereAttack()
{
}

void USphereAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	if (!InOwner) { return; }

	UAttackBase::Initialize(InOwner);

	// 攻撃用のSphereCollisionアクターの設定
	{
		SphereCollision = GetWorld()->SpawnActor<ASphereCollision>(SubSphereCollision);
		if (!SphereCollision) { return; }
		SphereCollision->BeginPlay();

		// 攻撃用スフィアアクターに親をアタッチ
		SphereCollision->AttachToActor(
			InOwner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 攻撃の最大時間を加算して「攻撃の最大有効時間」として使う
		AddDuration(SphereCollision->GetBaseAttackDuration());

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		SphereCollision->SetAllCollisionResponseToChannel(
			GetTargetChannel(),
			GetTargetResponse()
		);

		// オーバーラップ時、オーバーラップ抜け時の関数をセット
		SphereCollision->BindSphereBeginOverlapEvents<ThisClass>(
			this
			, &ThisClass::OnSphericalBeginOverlap
		);
		SphereCollision->BindSphereEndOverlapEvents<ThisClass>(
			this
			, &ThisClass::OnSphericalEndOverlap
		);

		Cancel();
	}
}

void USphereAttack::Execute()
{
	UCollisionAttack::Execute();
	
	if (!SphereCollision) { return; }
	SphereCollision->SetJustExecuteTime(GetJustExecuteTime());	// 有効化された瞬間の時間をセット
	SphereCollision->SetActorTransform(GetOffsetTransform());	// 補正用トランスフォーム値をコリジョンの基準としてセット
}

void USphereAttack::Cancel()
{
	UCollisionAttack::Cancel();

	if (!SphereCollision) { return; }
	SphereCollision->SetAllCollisionEnabled(ECollisionEnabled::NoCollision); // 当たり判定無効化
}

void USphereAttack::Update(float InDeltaTime)
{
	if (!IsActive()) { return; }

	UCollisionAttack::Update(InDeltaTime);

	if (!IsValid(SphereCollision)) { return; }
	SphereCollision->Update(InDeltaTime);
}

void USphereAttack::ApplyCollisionTransform(const FTransform& InTransform)
{
	if (!IsValid(SphereCollision)) { return; }
	
	SphereCollision->SetActorTransform(InTransform);
}