
#include "LaserGearState_Lv2.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

#include <ProjectNull/System/Combat/Shooter/LaserBulletShooter/LaserBulletShooter.h>

ULaserGearState_Lv2::ULaserGearState_Lv2()
{
}

void ULaserGearState_Lv2::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	ULaserGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);

	if (!ReflectiveLaserBulletShooter) { return; }
	ReflectiveLaserBulletShooter->Initialize(InPlayer);
}

void ULaserGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	if (!Player) { return; }

	ULaserGearStateBase::Execute(CurrentGearLevel);

	if (!ReflectiveLaserBulletShooter) { return; }

	ReflectiveLaserBulletShooter->ShotTargetedLaserBullets(Player->GetActorLocation());
	
	/** 効果音 */
	if (GearSESound.IsValidIndex(SEIndex::LaserGunSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::LaserGunSESoundIndex]);
	}
}

void ULaserGearState_Lv2::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);



}

void ULaserGearState_Lv2::End()
{
	ULaserGearStateBase::End();
	if (!ReflectiveLaserBulletShooter) { return; }
	ReflectiveLaserBulletShooter->Reset();
}
