
#include "LaserGearState_Lv1.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

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

	for (int32 Num = 0; Num < LaserBulletNum; ++Num) 
	{
		ShotLaserBullet();
	}
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

	
	GetWorld()->SpawnActor<AProjectileBase>(
			LaserBulletClass,
			SpawnLocation,
			SpawnRotation,
			SpawnParams);
}
