
#include "LaserGearState_Lv1.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>


#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>

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

	
}

void ULaserGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);

	ShotTargetedLaserBullets(BulletData);
	
}

void ULaserGearState_Lv1::Update(float DeltaTime)
{
	if (!Player) { return; }

	ULaserGearStateBase::Update(DeltaTime);

	for (int32 Num = 0; Num < InBulletData.Num; ++Num)
	{
		ShotLaserBullet(InBulletData, SpawnLocation);
	}

	if (bDrawDebugLine)
	{
		DrawDebugSphere(
			GetWorld(),
			Player->GetActorLocation(),
			FMath::Sqrt(BulletData.TargetableDistSq),
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


