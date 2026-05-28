
#include "LaserGearState_Lv1.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

ULaserGearState_Lv1::ULaserGearState_Lv1():
	TargetableDistSq(0.f)
{

}

void ULaserGearState_Lv1::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	ULaserGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);

	
}

void ULaserGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);


	if (!Player) { return; }

	const FVector SpawnLocation = Player->GetActorLocation();

	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies = TargetSearch->FindEnemiesSortedByDistance(TargetableDistSq);

	TArray<FVector> VelocityArray;

	VelocityArray.Init(Player->GetActorForwardVector(), BulletData.Num);

	for (int32 Index = 0; Index < BulletData.Num && Index < Enemies.Num(); ++Index)
	{
		VelocityArray[Index] = Enemies[Index].ToEnemyVector;
	}

	for (int32 Num = 0; Num < BulletData.Num; ++Num)
	{
		ShotLaserBullet(SpawnLocation,VelocityArray[Num]);
	}

}

void ULaserGearState_Lv1::Update(float DeltaTime)
{
	if (!Player) { return; }

	ULaserGearStateBase::Update(DeltaTime);


	if (bDrawDebugLine)
	{
		DrawDebugSphere(
			GetWorld(),
			Player->GetActorLocation(),
			FMath::Sqrt(TargetableDistSq),
			16,
			FColor::Green,
			false,
			0.1f);
	}


}

void ULaserGearState_Lv1::End()
{
	ULaserGearStateBase::End();


}

void ULaserGearState_Lv1::ShotLaserBullet(
	const FVector& SpawnLocation,
	const FVector& DirVector)
{
	auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(
		BulletData.Class);

	if (!LaserBullet) { return; }

	LaserBullet->SetOwnerActor(Player);
	LaserBullet->SetActorLocation(SpawnLocation);

	auto ProjectileMovement = LaserBullet->GetProjectileMovement();

	if (!ProjectileMovement) { return; }

	ProjectileMovement->Velocity = DirVector * BulletData.Speed;

	/*UE_LOG(LogTemp, Display, TEXT("pvelo x:%.1f,y:%.1f,z:%.1f"),
		ProjectileMovement->Velocity.X,
		ProjectileMovement->Velocity.Y,
		ProjectileMovement->Velocity.Z);*/
}

