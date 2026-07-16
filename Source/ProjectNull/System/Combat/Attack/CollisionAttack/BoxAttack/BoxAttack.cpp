#include "BoxAttack.h"

#include "Components\BoxComponent.h"

#include <ProjectNull\Actor\CollisionActor\BoxCollision\BoxCollision.h>

UBoxAttack::UBoxAttack()
{
}

void UBoxAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	if (!InOwner) { return; }

	UAttackBase::Initialize(InOwner);

	// 攻撃用のBoxCollisionアクターの設定
	{
		BoxCollision = GetWorld()->SpawnActor<ABoxCollision>(SubBoxCollision);
		if (!BoxCollision) { return; }

		// 攻撃用ボックスアクターに親をアタッチ
		BoxCollision->AttachToActor(
			InOwner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		BoxCollision->SetCollisionResponseToChannnel(
			GetTargetChannel(),
			GetTargetResponse()
		);

		if (auto BoxComponent = BoxCollision->GetBoxComponent())
		{
			// オーバーラップ時、オーバーラップ抜け時の関数をセット
			BoxComponent->OnComponentBeginOverlap.AddDynamic(
				this,
				&ThisClass::OnCollisionBeginOverlap
			);
			BoxComponent->OnComponentEndOverlap.AddDynamic(
				this,
				&ThisClass::OnCollisionEndOverlap
			);
		}

		Cancel();
	}
}

void UBoxAttack::Execute(const FVector& InTargetLocation)
{
	UCollisionAttack::Execute(InTargetLocation);

	if (!BoxCollision) { return; }
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);// 当たり判定有効化	
	BoxCollision->SetRelativeTransform(GetOffsetTransform());		// 初期Transformに設定
}

void UBoxAttack::Cancel()
{
	UCollisionAttack::Cancel();

	if (!BoxCollision) { return; }
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void UBoxAttack::Update(const float InDeltaTime)
{
	if (!IsActive()) { return; }

	UCollisionAttack::Update(InDeltaTime);
}