#include "BoxAttack.h"

#include "Components\BoxComponent.h"

#include <ProjectNull\Actor\CollisionActor\BoxCollision\BoxCollision.h>
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

UBoxAttack::UBoxAttack()
	: Duration(1.f)
	, ElpsedTimer(0.f)
{
}

void UBoxAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	if (!Owner) { return; }

	UAttackBase::Initialize(Owner);

	// 攻撃用のSphereCollisionアクターの設定
	{
		BoxCollision = GetWorld()->SpawnActor<ABoxCollision>(SubBoxCollision);
		if (!BoxCollision) { return; }

		// 攻撃用球アクターに親をアタッチ
		BoxCollision->AttachToActor(
			Owner,
			FAttachmentTransformRules::KeepRelativeTransform
		);

		// 指定したコリジョンチャンネルとそれに対するレスポンスをセット
		BoxCollision->SetCollisionResponseToChannnel(
			CollisionChannel,
			CollisionResponse
		);

		if (auto BoxComponent = BoxCollision->GetSphereComponent())
		{
			// オーバーラップ時、オーバーラップ抜け時の関数をセット
			BoxComponent->OnComponentBeginOverlap.AddDynamic(
				this,
				&UBoxAttack::OnSphericalBeginOverlap
			);
			BoxComponent->OnComponentEndOverlap.AddDynamic(
				this,
				&UBoxAttack::OnSphericalEndOverlap
			);
		}

		Cancel();
	}
}

void UBoxAttack::Execute()
{
	if (!BoxCollision) { return; }

	SetIsActive(true);		// 攻撃有効化
	SetCanExecute(false);	// 攻撃実行不可にする
	ElpsedTimer = 0.f;		// 経過時間をリセット
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);	// 当たり判定有効化	
	BoxCollision->SetRelativeTransform(GetOffsetTransform());		// 初期Transformに設定
}

void UBoxAttack::Cancel()
{
	if (!BoxCollision) { return; }

	SetIsActive(false);		// 攻撃無効化
	ElpsedTimer = 0.f;		// 経過時間をリセット
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);	// 当たり判定無効化
}

void UBoxAttack::Update(float DeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive() || !BoxCollision) { return; }

	UAttackBase::Update(DeltaTime);

	// 継続時間を超えたら攻撃を終了
	if (Duration < ElpsedTimer)
	{
		Cancel();
		return;
	}

	if (auto BoxComponent = BoxCollision->GetSphereComponent())
	{
		DrawDebugBox(
			GetWorld(),								// ワールドポインタ
			BoxComponent->GetComponentLocation(),	// 中心のワールド座標
			BoxComponent->GetScaledBoxExtent(),		// 範囲
			FColor::Red,							// 色
			false,									// 永続的に表示するか
			1.f,									// 表示時間（負の値で1フレーム）
			0,										// 優先度
			2.f										// 線の太さ
		);
	}

	ElpsedTimer += DeltaTime;
}

void UBoxAttack::OnSphericalBeginOverlap(
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

void UBoxAttack::OnSphericalEndOverlap(
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