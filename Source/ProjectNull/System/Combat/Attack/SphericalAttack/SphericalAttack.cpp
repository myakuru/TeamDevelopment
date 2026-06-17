#include "SphericalAttack.h"

#include "Components\SphereComponent.h"

#include <ProjectNull\Actor\CollisionActor\SphereCollision\SphereCollision.h>
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
	
	// 攻撃用のSphereCollisionアクターの設定
	{
		SphereCollision = GetWorld()->SpawnActor<ASphereCollision>(SubSphereCollision);
		if (!SphereCollision) { return; }
		
		// 攻撃用球アクターに親をアタッチ
		SphereCollision->AttachToActor(
			Owner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		SphereCollision->SetCollisionResponseToChannnel(
			CollisionChannel,
			CollisionResponse
		);

		if (auto SphereComponent = SphereCollision->GetSphereComponent())
		{
			// オーバーラップ時、オーバーラップ抜け時の関数をセット
			SphereComponent->OnComponentBeginOverlap.AddDynamic(
				this,
				&USphericalAttack::OnSphericalBeginOverlap
			);
			SphereComponent->OnComponentEndOverlap.AddDynamic(
				this,
				&USphericalAttack::OnSphericalEndOverlap
			);
		}

		Cancel();
	}
}

void USphericalAttack::Execute()
{
	if (!SphereCollision) { return; }

	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 当たり判定有効化	
	SphereCollision->SetRelativeTransform(GetOffsetTransform());		// 初期Transformに設定
}

void USphericalAttack::Cancel()
{
	if (!SphereCollision) { return; }

	SetIsActive(false);		// 攻撃無効化
	ElpsedTimer = 0.f;		// 経過時間をリセット
	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void USphericalAttack::Update(float DeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive()||!SphereCollision) { return; }

	UAttackBase::Update(DeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

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