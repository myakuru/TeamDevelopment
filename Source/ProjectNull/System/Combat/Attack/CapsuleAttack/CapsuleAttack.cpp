#include "CapsuleAttack.h"

#include "Components\CapsuleComponent.h"

#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

UCapsuleAttack::UCapsuleAttack()
	:	Duration(1.f)
	,	ElpsedTimer(0.f)
{
}

void UCapsuleAttack::Initialize(const TObjectPtr<AActor>& InOwner)
{
	if (!InOwner) { return; }

	UAttackBase::Initialize(InOwner);

	// 攻撃用のCapsuleComponentを生成
	{
		AttackCapsule = NewObject<UCapsuleComponent>(InOwner);

		if (GetRootComponent())
		{
			AttackCapsule->SetupAttachment(GetRootComponent());
		}
		AttackCapsule->RegisterComponent();
		AttackCapsule->SetCollisionResponseToChannel(
			ECC_GameTraceChannel1,
			ECR_Overlap
		);
		AttackCapsule->OnComponentBeginOverlap.AddDynamic(
			this,
			&UCapsuleAttack::OnSphericalBeginOverlap
		);
		AttackCapsule->OnComponentEndOverlap.AddDynamic(
			this,
			&UCapsuleAttack::OnSphericalEndOverlap
		);
	}
}

void UCapsuleAttack::Execute()
{
	if (!AttackCapsule) { return; }

	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 当たり判定有効化
	AttackCapsule->SetRelativeTransform(GetOffsetTransform());			// 初期Transformに設定
}

void UCapsuleAttack::Cancel()
{
	if (!AttackCapsule) { return; }

	SetIsActive(false);		// 攻撃無効化
	SetCanExecute(true);	// 攻撃実行可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
	AttackCapsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void UCapsuleAttack::Update(float DeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive() || !AttackCapsule) { return; }

	UAttackBase::Update(DeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

	ElpsedTimer += DeltaTime;
}

void UCapsuleAttack::OnSphericalBeginOverlap(
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
			interface->ApplyDamaged();
			interface->ApplyKnockBack(GetOwnerActor()->GetActorLocation());
			AddHitActors(OtherActor);
		}
	}
}

void UCapsuleAttack::OnSphericalEndOverlap(
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