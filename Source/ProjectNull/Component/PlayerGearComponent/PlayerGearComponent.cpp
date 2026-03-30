#include "PlayerGearComponent.h"

#include "../../System/Gear/GearBase.h"

UPlayerGearComponent::UPlayerGearComponent():
	CurrentGearLevel(1)
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerGearComponent::BeginPlay()
{
	Super::BeginPlay();

	for (auto& gear : PlayerGears)
	{
		if (!gear) { continue; }
		gear->Initialize(OwnerPlayer,this);
	}
	
}

void UPlayerGearComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	for(auto& gear : PlayerGears)
	{
		if (!gear) { continue; }

		if(gear->CanExecute())
		{
			gear->Execute();
		}

		gear->Update(DeltaTime);

	}

}

