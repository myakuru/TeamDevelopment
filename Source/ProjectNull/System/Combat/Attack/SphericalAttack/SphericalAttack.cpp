#include "SphericalAttack.h"

#include "Components\SphereComponent.h"

#include <ProjectNull\Utility\Common\Definitions\CollisionChannels.h>
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>
#include "SphericalAttack.h"

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
		
		if (GetRootComponent())
		{
			AttackSphere->SetupAttachment(GetRootComponent());
		}
		AttackSphere->RegisterComponent();

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

	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 当たり判定有効化
	AttackSphere->SetRelativeTransform(GetOffsetTransform());			// 初期Transformに設定
}

void USphericalAttack::Cancel()
{
	if (!AttackSphere) { return; }

	SetIsActive(false);		// 攻撃無効化
	ElpsedTimer = 0.f;		// 経過時間をリセット
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void USphericalAttack::Update(float DeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive()||!AttackSphere) { return; }

	UAttackBase::Update(DeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

	DrawDebugSphere(
		GetWorld(),             // ワールドポインタ
		AttackSphere->GetComponentLocation(),     // 中心のワールド座標
		AttackSphere->GetScaledSphereRadius(),                  // 半径
		12.f,                     // セグメント数（円の滑らかさ）
		FColor::Red,            // 色
		false,                  // 永続的に表示するかどうか
		1.f,                  // 表示時間（負の値で1フレーム）
		0,                      // 優先度
		10.0f                    // 線の太さ
	);

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
	if (!OtherActor || !GetOwnerActor()
		|| OtherActor == GetOwnerActor()) { return; }
	
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

void USphericalAttack::OnSphericalEndOverlap(
	UPrimitiveComponent* OverlappedComponent, 
	AActor* OtherActor, 
	UPrimitiveComponent* OtherComp, 
	int32 OtherBodyIndex
)
{
	if (!OtherActor || !GetOwnerActor()
		|| OtherActor == GetOwnerActor()) {return;}

	if (GetHitActors().Contains(OtherActor))
	{
		RemoveActor(OtherActor);
	}
}