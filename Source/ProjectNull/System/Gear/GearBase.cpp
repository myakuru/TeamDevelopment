#include "GearBase.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>


UGearBase::UGearBase():
	OwnerPlayer(nullptr),
	OwnerGearComponent(nullptr),
	GearStates(TArray<UGearStateBase*>()),
	GearStatuses(TArray<FGearStatus>()),
	CurrentGearState(nullptr),
	ExecutedGearLevel(1),
	bCanExecute(true),
	ElapsedTime(0.f),
	Duration(0.f),
	bIsActive(false),
	bBlocksMovement(false),
	DurationTimerHandle(FTimerHandle())
{

}

void UGearBase::Initialize(
	APlayerBase* Player,
	UPlayerGearComponent* GearComponent)
{
	OwnerPlayer			= Player;
	OwnerGearComponent	= GearComponent;

	for (auto& State : GearStates)
	{
		if (!State) { continue; }

		State->Initialize(Player, GearComponent, this);
	}

}

void UGearBase::Execute(int32 CurrentGearLevel)
{
	if (!bCanExecute) { return; }

	bIsActive			= true;
	bCanExecute			= false;
	ExecutedGearLevel	= CurrentGearLevel;

	// 発動したいギアの状態クラス配列のインデックス取得
	const int32	StateIndex = ExecutedGearLevel - 1;

	// 発動したいギアの状態クラスを
	// 現在のギアの状態クラスとする
	CurrentGearState = GearStates.IsValidIndex(StateIndex) ? GearStates[StateIndex] : nullptr;

	if (!CurrentGearState)						{ return; }
	if (!GearStatuses.IsValidIndex(StateIndex)) { return; }

	// 発動時間の更新
	Duration = GearStatuses[StateIndex].Duration;

	// ギアのクールタイムをセットし、クールタイム終了時にリセット処理を呼ぶ
	GetWorld()->GetTimerManager().SetTimer(
		DurationTimerHandle,
		this,
		&UGearBase::Reset,
		GearStatuses[StateIndex].CoolTime,
		false);

	// 状態クラス実行処理
	CurrentGearState->Execute(CurrentGearLevel);
}

void UGearBase::Update(float DeltaTime)
{
	if (!bIsActive)			{ return; }
	if (!CurrentGearState)	{ return; }

	// 経過時間更新
	ElapsedTime += DeltaTime;

	// 状態クラスの更新
	CurrentGearState->Update(DeltaTime);
	
	// 発動時間が終了したら
	// 状態クラスの終了処理を呼び出し、更新を行わない
	if (ElapsedTime >= Duration)
	{
		CurrentGearState->End();
		bIsActive	= false;
		ElapsedTime = 0.0f;
	}
}

float UGearBase::GetGearDuration(int32 Index) const
{
	if (!GearStatuses.IsValidIndex(Index)) { return 0.f; }
	return GearStatuses[Index].Duration;
}

void UGearBase::SetGearDuration(float InDuration, int32 Index)
{
	if (!GearStatuses.IsValidIndex(Index)) { return; }
	GearStatuses[Index].Duration = InDuration;
}

void UGearBase::Reset()
{
	bCanExecute = true;
}

