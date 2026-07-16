#include "DashGear.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/System/Combat/Attack/CollisionAttack/CollisionAttack.h>

// 備忘録
// 元は攻撃スケーラーという倍率変数があったが、
// 攻撃の威力を変更して対応する

UDashGear::UDashGear()
	:	SphereAttacks(TArray<TObjectPtr<UCollisionAttack>>())
{
}

void UDashGear::Initialize(
	APlayerBase* Player,
	UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(
		Player,
		GearComponent);

	if (!IsValid(OwnerPlayer)) { return; }

	if (SphereAttacks.IsEmpty()) { return; }
	for (const auto& Attack : SphereAttacks)
	{
		if (!IsValid(Attack)) { continue; }
		Attack->Initialize(OwnerPlayer);
	}
}

void UDashGear::Execute(int32 CurrentGearLevel)
{
	UGearBase::Execute(CurrentGearLevel);
	
	// 発動したいギアステートに該当する攻撃のインデックス
	CurrentExecuteAttackIndex = CurrentGearLevel - 1;
	
	// 攻撃有効化
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	SphereAttacks[CurrentExecuteAttackIndex]->Execute();
}

void UDashGear::Update(float DeltaTime)
{
	UGearBase::Update(DeltaTime);

	if (!IsActive())
	{
		SetBlocksMovement(false);
	}
	
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	SphereAttacks[CurrentExecuteAttackIndex]->Update(DeltaTime);
}

void UDashGear::SetSphereTransform(const FTransform& Transform) const
{
	// コリジョンのトランスフォームを指定の値を基に変化させる
	if (!SphereAttacks.IsValidIndex(CurrentExecuteAttackIndex)) { return; }
	UE_LOG(LogTemp,Warning,TEXT("SetActorLocation!!"));
	SphereAttacks[CurrentExecuteAttackIndex]->ApplyCollisionTransform(Transform);
}

void UDashGear::OnDashGearAttackBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!IsValid(OtherActor)) { return; }
	const auto Interface = Cast<ICharacterInterface>(OtherActor);

	// TargetのActorのローカル座標を取得
	FVector HitWorldLocation = OwnerPlayer->GetActorLocation();

	if (IsValid(OtherComp))
	{
		FVector ClosestPoint;
		// @brief	当たった相手のコリジョンで、OwnerPlayerの位置に一番近い点を探す
		// ClosesPoint	OtherCompのコリジョン表面上で、プレイヤーに一番近いワールド座標
		// Distance		Player位置からClosesPointまでの距離
		const float Distance = OtherComp->GetClosestPointOnCollision(
			OwnerPlayer->GetActorLocation(),
			ClosestPoint
		);

		// Player位置とOtherCompのコリジョンにきょりがあれば
		if (Distance >= 0.0f)
		{
			// Playerに一番近い敵のコリジョンの表面を当たった座標にセット
			HitWorldLocation = ClosestPoint;
		}
		else
		{
			HitWorldLocation = OtherActor->GetActorLocation();
		}
	}
	else
	{
		HitWorldLocation = OtherActor->GetActorLocation();
	}

	Interface->ApplyLocalHitPos(HitWorldLocation);
}