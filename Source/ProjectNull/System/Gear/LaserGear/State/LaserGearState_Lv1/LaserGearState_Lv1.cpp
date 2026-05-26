
#include "LaserGearState_Lv1.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

ULaserGearState_Lv1::ULaserGearState_Lv1():
	LaserBulletClass(nullptr),
	LaserBulletNum(1)
{

}

void ULaserGearState_Lv1::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	ULaserGearStateBase::Initialize(InPlayer, InGearComponent, InOwner);

	
}

void ULaserGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);

	ShotLaserBullet();

}

void ULaserGearState_Lv1::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);

}

void ULaserGearState_Lv1::End()
{
	ULaserGearStateBase::End();


}

void ULaserGearState_Lv1::ShotLaserBullet()
{
	if (!Player) { return; }

	const FVector SpawnLocation = Player->GetActorLocation();
	const FRotator SpawnRotation = Player->GetActorRotation();

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = Player;
	SpawnParams.Instigator = Player->GetInstigator();

	
	const auto TargetSearch = Player->GetTargetSearchComponent();
	if (!TargetSearch) { return; }

	const TArray<FEnemyDistanceData> Enemies = TargetSearch->FindEnemiesSortedByDistance(2000.0f);

	if (!Enemies.IsEmpty()) {

		for (int32 Index = 0; Index < LaserBulletNum && Index < Enemies.Num(); ++Index)
		{
			auto LaserBullet = GetWorld()->SpawnActor<AProjectileBase>(
				LaserBulletClass,
				SpawnLocation,
				SpawnRotation,
				SpawnParams);

			if (!LaserBullet) { continue; }

			auto ProjectileMovement = LaserBullet->GetProjectileMovement();

			if (!ProjectileMovement) { continue; }

			ProjectileMovement->Velocity = Enemies[Index].ToEnemyVector;
		}
	}






	
}
