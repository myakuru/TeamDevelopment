#include "PlayerGearComponent.h"

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>


UPlayerGearComponent::UPlayerGearComponent()
	:	OwnerPlayer(nullptr)
	,	PlayerGears(TArray<UGearBase*>())
	,	CurrentGearLevel(1)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerGearComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& Gear : PlayerGears)
	{
		if (!Gear) { continue; }
		Gear->Initialize(OwnerPlayer,this);
	}
}

void UPlayerGearComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(auto& Gear : PlayerGears) {
		if (!Gear) { continue; }
		Gear->Update(DeltaTime);
	}

}

bool UPlayerGearComponent::IsMovementBlockedByGear() const
{
	for (auto& Gear : PlayerGears) {
		if (!Gear) { continue; }
		if (Gear->BlocksMovement()) {
			return true;
		}
	}
	return false;
}

void UPlayerGearComponent::ExecuteGear(int32 GearIndex)
{
	if (PlayerGears.IsValidIndex(GearIndex)) {
		if (PlayerGears[GearIndex]) {
			PlayerGears[GearIndex]->Execute(CurrentGearLevel);
		}
	}
}

void UPlayerGearComponent::ChangeGear()
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance() || !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()) {
		return;
	}
	if (!CanChangeGear()) { return; }
	const TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData = OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData();
	PlayerRuntimeData->ResetDataOnGearChange(CurrentGearLevel);
	CurrentGearLevel = ++CurrentGearLevel % 4;
}

bool UPlayerGearComponent::CanChangeGear() const
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance() || !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()) {
		return false;
	}

	const TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData = OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData();
	return PlayerRuntimeData->CanChangeGear(CurrentGearLevel);
}

