#include "CollisionAttack.h"
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

UCollisionAttack::UCollisionAttack()
	:	Duration(1.f)
	,	ElpsedTimer(0.f)
{
}

void UCollisionAttack::Update(const float InDeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive()) { return; }

	UAttackBase::Update(InDeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

	ElpsedTimer += InDeltaTime;
}

void UCollisionAttack::Execute()
{
	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
}

void UCollisionAttack::Cancel()
{
	SetIsActive(false);		// 攻撃無効化
	ElpsedTimer = 0.f;		// 経過時間をリセット
}

bool UCollisionAttack::CanStartAttack()
{
	return false;
}

void UCollisionAttack::OnSphericalBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || !GetOwnerActor()
		|| OtherActor == GetOwnerActor()) {
		return;
	}

	if (!GetHitActors().Contains(OtherActor))
	{
		// キャラクターインターフェースを実装しているか
		if (auto* interface = Cast<ICharacterInterface>(OtherActor))
		{
			interface->ApplyDamaged(GetFinalDamage());
			interface->ApplyKnockBack(GetOwnerActor()->GetActorLocation());
			AddHitActors(OtherActor);
		}
	}
}

void UCollisionAttack::OnSphericalEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor || !GetOwnerActor()
		|| OtherActor == GetOwnerActor()) {
		return;
	}

	if (GetHitActors().Contains(OtherActor))
	{
		RemoveActor(OtherActor);
	}
}