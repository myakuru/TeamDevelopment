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

	// �M�A�̏�����
	for (auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		gear->Initialize(OwnerPlayer,this);
	}
}

void UPlayerGearComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// �M�A�̍X�V
	for(auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		gear->Update(DeltaTime);
	}

}

bool UPlayerGearComponent::IsMovementBlockedByGear() const
{
	// �M�A�̒��Ɉړ���u���b�N�����̂����邩�ǂ����𔻒�
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

