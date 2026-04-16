#include "PlayerGearComponent.h"

#include "ProjectNull\System\Gear\GearBase.h"


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

	// ギアの初期化
	for (auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		gear->Initialize(OwnerPlayer,this);
	}
}

void UPlayerGearComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ギアの更新
	for(auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		gear->Update(DeltaTime);
	}

}

bool UPlayerGearComponent::IsMovementBlockedByGear() const
{
	// ギアの中に移動をブロックするものがあるかどうかを判定
	for (auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		if (gear->BlocksMovement())
		{
			return true;
		}
	}
	return false;
}

void UPlayerGearComponent::ExecuteGear(int32 GearIndex)
{
	if (PlayerGears.IsValidIndex(GearIndex))
	{
		if (PlayerGears[GearIndex])
		{
			PlayerGears[GearIndex]->Execute(CurrentGearLevel);
		}
	}
}

