#include "CollisionAttack.h"
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

UCollisionAttack::UCollisionAttack()
	:	Duration(1.f)
	,	JustExecuteTime(0.f)
{
}

void UCollisionAttack::Update(const float InDeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive()) { return; }

	UAttackBase::Update(InDeltaTime);

	// 発動した瞬間から有効時間分経過していたら終了
	if (GetWorld()->GetTimeSeconds() > Duration + JustExecuteTime)
	{
		Cancel();
		return;
	}
}

void UCollisionAttack::Execute()
{
	SetIsActive(true);								// 攻撃有効化
	SetCanExecute(false);							// 攻撃実行不可にする
	JustExecuteTime = GetWorld()->GetTimeSeconds();	// 「発動した時間」として現在の時間を保存
}

void UCollisionAttack::Cancel()
{
	SetIsActive(false);		// 攻撃無効化
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
			
			// 攻撃がHITした瞬間のデリゲートの発火
			OnOverlapInDelegate.Broadcast(OtherActor);
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
		
		// HIT判定から抜け出した瞬間のデリゲートの発火
		OnOverlapOutDelegate.Broadcast(OtherActor);
	}
}