
#include "LaserGearState_Lv1.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/System/Combat/Shooter/LaserBulletShooter/LaserBulletShooter.h>


ULaserGearState_Lv1::ULaserGearState_Lv1()
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

	if (!LaserBulletShooter) { return; }
	LaserBulletShooter->Initialize(InPlayer);
}

void ULaserGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	if (!Player) { return; }

	ULaserGearStateBase::Execute(CurrentGearLevel);

	if (!LaserBulletShooter) { return; }

	LaserBulletShooter->ShotTargetedLaserBullets(Player->GetActorLocation());
	
	/** 効果音 */
	if (GearSESound.IsValidIndex(SEIndex::LaserGunSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::LaserGunSESoundIndex]);
	}
}

void ULaserGearState_Lv1::Update(float DeltaTime)
{
	if (!Player) { return; }

	ULaserGearStateBase::Update(DeltaTime);

	if (!LaserBulletShooter) { return; }


	if (bDrawDebugLine)
	{
		DrawDebugSphere(
			GetWorld(),
			Player->GetActorLocation(),
			FMath::Sqrt(LaserBulletShooter->GetTargetableDistSq()),
			16,
			FColor::Green,
			false,
			0.1f);
	}


}

void ULaserGearState_Lv1::End()
{
	ULaserGearStateBase::End();
	if (!LaserBulletShooter) { return; }
	LaserBulletShooter->Reset();
}


