#include "SphericalAttack.h"

#include "Components\SphereComponent.h"

#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

USphericalAttack::USphericalAttack()
	:	Duration(1.f)
	,	ElpsedTimer(0.f)
{
}

void USphericalAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	if (!Owner) { return; }

	UAttackBase::Initialize(Owner);

	// 攻撃用のSphereComponentを生成
	{
		AttackSphere = NewObject<USphereComponent>(Owner);
		
		if (RootComponent)
		{
			AttackSphere->SetupAttachment(RootComponent);
		}
		AttackSphere->RegisterComponent();
		AttackSphere->SetCollisionResponseToChannel(
			ECC_GameTraceChannel1,
			ECR_Overlap
		);
		AttackSphere->OnComponentBeginOverlap.AddDynamic(
			this,
			&USphericalAttack::OnSphericalBeginOverlap
		);
		AttackSphere->OnComponentEndOverlap.AddDynamic(
			this,
			&USphericalAttack::OnSphericalEndOverlap
		);
	}
	Cancel();
}

void USphericalAttack::Execute()
{
	if (!AttackSphere) { return; }

	bIsActive = true;	// 攻撃有効化
	bCanExecute = false;// 攻撃実行不可にする
	ElpsedTimer = 0.f;	// 経過時間をリセット
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 当たり判定有効化
	AttackSphere->SetRelativeTransform(OffsetTransform);				// 初期Transformに設定
}

void USphericalAttack::Cancel()
{
	if (!AttackSphere) { return; }

	bIsActive = false;	// 攻撃無効化
	bCanExecute = true;	// 攻撃実行可にする
	ElpsedTimer = 0.f;	// 経過時間をリセット
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void USphericalAttack::Update(float DeltaTime)
{
	// 有効時以外は処理しない
	if (!bIsActive||!AttackSphere) { return; }

	UAttackBase::Update(DeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

	ElpsedTimer += DeltaTime;
}

void USphericalAttack::OnSphericalBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep, 
	const FHitResult& SweepResult
)
{
	if (!OtherActor || !OwnerActor
		|| OtherActor == OwnerActor) { return; }
	
	if (!HitActors.Contains(OtherActor))
	{	
		// キャラクターインターフェースを実装しているか
		if (auto* interface = Cast<ICharacterInterface>(OtherActor))
		{
			interface->TakeDamaged();
			interface->TakeKnockBack(OwnerActor->GetActorLocation());
			HitActors.Add(OtherActor);
		}
	}
}

void USphericalAttack::OnSphericalEndOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex
)
{
	if (!OtherActor || !OwnerActor
		|| OtherActor == OwnerActor) {return;}

	if (HitActors.Contains(OtherActor))
	{
		HitActors.Remove(OtherActor);
	}
}