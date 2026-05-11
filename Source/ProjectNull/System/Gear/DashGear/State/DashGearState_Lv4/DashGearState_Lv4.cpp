
#include "DashGearState_Lv4.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Gear/GearBase.h>

UDashGearState_Lv4::UDashGearState_Lv4()
{
}

void UDashGearState_Lv4::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent, UGearBase* Gear)
{
	UDashGearStateBase::Initialize(Player, GearComponent, Gear);

	if (!Player || !Gear) { return; }
	StartPlayerForwardVector = Player->GetActorForwardVector();

	float Duration = 0.0f;

	for (auto& Data : DashSpecialMoveDataArray) {
		Duration += Data.Time;
	}
	Gear->SetGearDuration(Duration, 3);
}

void UDashGearState_Lv4::Update(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerGear) { return; }

	float ElapsedTime = OwnerGear->GetElapsedTime();

	int32 ResultIndex = 0;
	for (int32 Index = 0; Index < DashSpecialMoveDataArray.Num(); ++Index) {
		ElapsedTime -= DashSpecialMoveDataArray[Index].Time;
		if (ElapsedTime <= 0.0f) {
			ResultIndex = Index;
			break;
		}
	}

	if (!DashSpecialMoveDataArray.IsValidIndex(ResultIndex)) { return; }
	const FDashSpecialMoveData DashSpecialMoveData = DashSpecialMoveDataArray[ResultIndex];
	const FVector Dir = FRotator(0.0f, DashSpecialMoveData.AngleOffset,0.0f).RotateVector(StartPlayerForwardVector).GetSafeNormal();
	const float Speed = DashSpecialMoveData.Speed;
	
	OwnerPlayer->SetActorRotation(Dir.Rotation());
	OwnerPlayer->LaunchCharacter(Dir * Speed, true, true);

}
