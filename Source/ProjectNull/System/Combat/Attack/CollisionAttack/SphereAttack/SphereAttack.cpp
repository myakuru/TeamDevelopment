#include "SphereAttack.h"

#include "Components\SphereComponent.h"

#include <ProjectNull\Actor\CollisionActor\SphereCollision\SphereCollision.h>

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

		// 攻撃用ボックスアクターに親をアタッチ
		SphereCollision->AttachToActor(
			InOwner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		SphereCollision->SetCollisionResponseToChannnel(
			GetTargetChannel(),
			GetTargetResponse()
		);

		if (auto SphereComponent = SphereCollision->GetSphereComponent())
		{
			// オーバーラップ時、オーバーラップ抜け時の関数をセット
			SphereComponent->OnComponentBeginOverlap.AddDynamic(
				this,
				&ThisClass::OnSphericalBeginOverlap
			);
			SphereComponent->OnComponentEndOverlap.AddDynamic(
				this,
				&ThisClass::OnSphericalEndOverlap
			);
		}

		Cancel();
	}
}

void USphereAttack::Execute()
{
	UCollisionAttack::Execute();

	if (!SphereCollision) { return; }
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 当たり判定有効化	
	SphereCollision->SetRelativeTransform(GetOffsetTransform());		// 初期Transformに設定
}

void USphereAttack::Cancel()
{
	UCollisionAttack::Cancel();

	if (!SphereCollision) { return; }
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化

}

void USphereAttack::Update(float InDeltaTime)
{
	if (!IsActive()) { return; }

	UCollisionAttack::Update(InDeltaTime);

	// デバッグ表示
	if (auto SphereComponent = SphereCollision->GetSphereComponent())
	{
		DrawDebugSphere(
			GetWorld(),									// ワールドポインタ
			SphereComponent->GetComponentLocation(),	// 中心のワールド座標
			SphereComponent->GetScaledSphereRadius(),	// 半径
			12.f,										// セグメント数（円の滑らかさ）
			FColor::Red,								// 色
			false,										// 永続的に表示するかどうか
			1.f,										// 表示時間（負の値で1フレーム）
			0,											// 優先度
			10.0f										// 線の太さ
		);
	}
}
