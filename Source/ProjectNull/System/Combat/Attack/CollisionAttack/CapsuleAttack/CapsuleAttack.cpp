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

	if (auto CapsuleComponent = CapsuleCollision->GetCapsuleComponent())
	{
		DrawDebugCapsule(
			GetWorld(),										// ワールドポインタ
			CapsuleComponent->GetComponentLocation(),		// 中心座標
			CapsuleComponent->GetScaledCapsuleHalfHeight(),	// 高さの半径
			CapsuleComponent->GetScaledCapsuleRadius(),		// 半径
			CapsuleComponent->GetComponentQuat(),			// 角度
			FColor::Red,									// 色
			false,											// 永続的に表示するか
			1.f,											// 表示時間(負の値で1フレーム)
			0,												// 優先度
			2.f												// 線の太さ
		);
	}
}
