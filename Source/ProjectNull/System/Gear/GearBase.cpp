#include "GearBase.h"

#include "ProjectNull/System/Gear/State/GearStateBase.h"
#include "ProjectNull/Actor/Character/Player/PlayerBase.h"
#include "ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h"


UGearBase::UGearBase():
	ExecutedGearLevel(1),
	ElapsedTime(0.0f),
	bCanExecute(true),
	bIsActive(false),
	bBlocksMovement(false)
{

}

void UGearBase::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	OwnerPlayer			= Player;
	OwnerGearComponent	= GearComponent;
}

void UGearBase::Execute(int32 CurrentGearLevel)
{
	if (!bCanExecute) { return; }

	bIsActive			= true;
	bCanExecute			= false;
	ExecutedGearLevel	= CurrentGearLevel;


	const int32		stateIndex		= ExecutedGearLevel - 1;
	UGearStateBase* currentState	= GearStates.IsValidIndex(stateIndex) ? GearStates[stateIndex] : nullptr;
	float			coolTime		= 0.0f;

	if (!currentState) { return; }

	if (GearStatuses.IsValidIndex(stateIndex))
	{
		coolTime = GearStatuses[stateIndex].CoolTime;
	}

	//　クールタイムの開始
	GetWorld()->GetTimerManager().SetTimer(
		DurationTimerHandle,
		this,
		&UGearBase::ResetParams,
		coolTime,
		false);


	currentState->Initialize(OwnerPlayer, OwnerGearComponent, this);
	currentState->Execute(CurrentGearLevel);
}

void UGearBase::Update(float DeltaTime)
{
	if (!bIsActive) { return; }

	ElapsedTime += DeltaTime;

	const int32		stateIndex		= ExecutedGearLevel - 1;
	UGearStateBase* currentState	= GearStates.IsValidIndex(stateIndex) ? GearStates[stateIndex] : nullptr;
	float			duration		= 0.0f;

	if (!currentState) { return; }

	currentState->Update(DeltaTime);
	
	if (GearStatuses.IsValidIndex(stateIndex)) 
	{
		duration = GearStatuses[stateIndex].Duration;
	}

	if (ElapsedTime >= duration)
	{
		currentState->End();
		bIsActive	= false;
		ElapsedTime = 0.0f;
	}
}

void UGearBase::ResetParams()
{
	bCanExecute = true;
}

