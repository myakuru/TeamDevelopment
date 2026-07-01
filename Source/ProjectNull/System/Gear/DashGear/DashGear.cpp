
#include "DashGear.h"

#include "Components/SphereComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/CollisionActor/SphereCollision/SphereCollision.h>

#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>

#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>


UDashGear::UDashGear():
	SphereCollision(nullptr),
	SphereCollisionClass(nullptr)
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
		Player,
		FAttachmentTransformRules::KeepRelativeTransform
	);

	auto SphereComponent = SphereCollision->GetSphereComponent();
	if (!SphereComponent) { return; }
	
	
	// オーバーラップ時、オーバーラップ抜け時の関数をセット
	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&UDashGear::OnDashGearAttackBeginOverlap
	);

	SetSphereCollisionEnabled(ECollisionEnabled::NoCollision);
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
	if (!OwnerPlayer) { return; }
	const FVector PlayerLocation = OwnerPlayer->GetActorLocation();

	auto Interface = Cast<ICharacterInterface>(OtherActor);
	if (!Interface) { return; }
	Interface->ApplyDamaged();
	Interface->ApplyKnockBack(PlayerLocation);
}

