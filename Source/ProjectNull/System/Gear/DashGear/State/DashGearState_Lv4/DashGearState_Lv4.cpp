
#include "DashGearState_Lv4.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/GhostActor/GhostActor.h>
#include <ProjectNull/Actor/Effect/AfterImageAttackEffect/AfterImageAttackEffect.h>
#include <ProjectNull/System/Gear/GearBase.h>

UDashGearState_Lv4::UDashGearState_Lv4()
{
}

void UDashGearState_Lv4::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UDashGearStateBase::Initialize(Player, GearComponent, Gear);

	if (!Player || !Gear) { return; }
	StartPlayerTransform = Player->GetTransform();

	if (!AfterImageAttackEffect) { return; }
	AfterImageAttackEffect->Initialize();
	const float Duration = AfterImageAttackEffect->GetMaxTime();
	Gear->SetGearDuration(Duration, kLv4Index);
}

void UDashGearState_Lv4::Execute(int32 CurrentGearLevel)
{
}

void UDashGearState_Lv4::Update(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerGear) { return; }

	if (!AfterImageAttackEffect) { return; }
	
	AfterImageAttackEffect->Update(DeltaTime, OwnerGear->GetElapsedTime(), StartPlayerTransform);
}
