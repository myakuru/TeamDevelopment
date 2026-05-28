
#include "LaserGearState_Lv2.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include <ProjectNull/Actor/Projectile/ProjectileBase.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>


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

	
}

void ULaserGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);



}

void ULaserGearState_Lv2::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);



}

void ULaserGearState_Lv2::End()
{
	ULaserGearStateBase::End();

}
