
#include "DashGear.h"

#include "Components/SphereComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/CollisionActor/SphereCollision/SphereCollision.h>

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>



UDashGear::UDashGear():
	SphereCollision(nullptr),
	SphereCollisionClass(nullptr),
	AttackPowerScale(1.f)
{
	
}

void UDashGear::Initialize(
	APlayerBase* Player,
	UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(
		Player,
		GearComponent);


	SphereCollision = GetWorld()->SpawnActor<ASphereCollision>
		(SphereCollisionClass);

	if (!SphereCollision) { return; }

	// 攻撃用球アクターに親をアタッチ
	SphereCollision->AttachToActor(
		OwnerPlayer,
		FAttachmentTransformRules::KeepRelativeTransform,
		TEXT("DashSphere")
	);

	auto SphereComponent = SphereCollision->GetSphereComponent();
	if (!SphereComponent) { return; }

	// オーバーラップ時、オーバーラップ抜け時の関数をセット
	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&UDashGear::OnDashGearAttackBeginOverlap
	);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }
	PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
	
}

void UDashGear::Execute(int32 CurrentGearLevel)
{
	
	UGearBase::Execute(CurrentGearLevel);
}

void UDashGear::Update(float DeltaTime)
{
	UGearBase::Update(DeltaTime);

	if (!IsActive()) 
	{
		SetBlocksMovement(false);
	}

}

void UDashGear::SetSphereCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (!SphereCollision) { return; }
	SphereCollision->SetCollisionEnabled(InEnabled);
}

void UDashGear::SetSphereTransform(const FTransform& Transform)
{
	if (!SphereCollision) { return; }
	SphereCollision->SetActorTransform(Transform);
}

void UDashGear::SetSphereRadius(float Radius)
{
	if (!SphereCollision) { return; }

	auto SphereComp = SphereCollision->GetSphereComponent();
	if (!SphereComp) { return; }

	SphereComp->SetSphereRadius(Radius);
}

void UDashGear::OnDashGearAttackBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	//UE_LOG(LogTemp, Display, TEXT("DashAttack"));

	if (!OwnerPlayer) { return; }
	const FVector PlayerLocation = OwnerPlayer->GetActorLocation();

	const auto Interface = Cast<ICharacterInterface>(OtherActor);
	if (!Interface) { return; }
	Interface->ApplyDamaged(PlayerRuntimeData->GetFinalAttackPower(AttackPowerScale));
	Interface->ApplyKnockBack(PlayerLocation);
	//Interface->ApplyLocalHitPos(OtherActor->GetActorLocation());



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

