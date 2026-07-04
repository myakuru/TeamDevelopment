#include "CapsuleAttack.h"

#include "Components\CapsuleComponent.h"

#include <ProjectNull\Actor\CollisionActor\CapsuleCollision\CapsuleCollision.h>

UCapsuleAttack::UCapsuleAttack()
{
}

void UCapsuleAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	if (!InOwner) { return; }

	UAttackBase::Initialize(InOwner);

	// 攻撃用のCapsuleCollisionアクターの設定
	{
		CapsuleCollision = GetWorld()->SpawnActor<ACapsuleCollision>(SubCapsuleCollision);
		if (!CapsuleCollision) { return; }

		// 攻撃用カプセルアクターに親をアタッチ
		CapsuleCollision->AttachToActor(
			InOwner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		CapsuleCollision->SetCollisionResponseToChannnel(
			GetTargetChannel(),
			GetTargetResponse()
		);

		if (auto CapsuleComponent = CapsuleCollision->GetCapsuleComponent())
		{
			// オーバーラップ時、オーバーラップ抜け時の関数をセット
			CapsuleComponent->OnComponentBeginOverlap.AddDynamic(
				this,
				&ThisClass::OnSphericalBeginOverlap
			);
			CapsuleComponent->OnComponentEndOverlap.AddDynamic(
				this,
				&ThisClass::OnSphericalEndOverlap
			);
		}

		Cancel();
	}
}

void UCapsuleAttack::Execute()
{
	UCollisionAttack::Execute();

	if (!CapsuleCollision) { return; }
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 当たり判定有効化	
	CapsuleCollision->SetRelativeTransform(GetOffsetTransform());		// 初期Transformに設定
}

void UCapsuleAttack::Cancel()
{
	UCollisionAttack::Cancel();

	if (!CapsuleCollision) { return; }
	CapsuleCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void UCapsuleAttack::Update(float InDeltaTime)
{
	if (!IsActive()) { return; }

	UCollisionAttack::Update(InDeltaTime);
}
