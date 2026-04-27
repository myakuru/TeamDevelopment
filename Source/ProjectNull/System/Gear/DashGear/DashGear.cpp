
#include "DashGear.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>
#include <ProjectNull/System/Gear/DashGear/State/DashGearState_Lv1/DashGearState_Lv1.h>

UDashGear::UDashGear()
{
}

void UDashGear::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	UGearBase::Initialize(Player, GearComponent);
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

