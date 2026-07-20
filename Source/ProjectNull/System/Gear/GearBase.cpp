#include "GearBase.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>

#include "ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h"
#include "ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h"


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
	SimultaneousActivationCoolTime(0.f),
	bIsActive(false),
	bBlocksMovement(false),
	CoolTimerHandle(FTimerHandle()),
	bAllowOtherGearActivation(true)
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
		State->SetGearSESound(GearSESound);
	}

	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance)		{ return; }

	PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
	
}

void UGearBase::Execute(int32 CurrentGearLevel)
{
	if (!bCanExecute)			{ return; }
	if (!OwnerGearComponent ||
		!PlayerRuntimeData)	{ return; }
	
	bIsActive			= true;
	bCanExecute			= false;
	ExecutedGearLevel	= CurrentGearLevel;
	bAllowOtherGearActivation = false;
	// 発動したいギアの状態クラス配列のインデックス取得
	const int32	StateIndex = ExecutedGearLevel - 1;

	// 発動したいギアの状態クラスを
	// 現在のギアの状態クラスとする
	CurrentGearState = GearStates.IsValidIndex(StateIndex) ? GearStates[StateIndex] : nullptr;

	if (!CurrentGearState)						{ return; }
	if (!GearStatuses.IsValidIndex(StateIndex)) { return; }

	// 発動時間の更新
	Duration = GearStatuses[StateIndex].Duration;
	SimultaneousActivationCoolTime = GearStatuses[StateIndex].SimultaneousActivationCoolTime;
	bAllowOtherGearActivation = false;
	
	float CoolTime = PlayerRuntimeData->IsInvincible() ?
	GearStatuses[StateIndex].CoolTime * OwnerGearComponent->GetCoolTimeScale():
	GearStatuses[StateIndex].CoolTime;
	
	CoolTime *= PlayerRuntimeData->GetEffectMultiplier(EUpgradeEffectType::PlayerGearCoolTime);
	
	// ギアのクールタイムをセットし、クールタイム終了時にリセット処理を呼ぶ
	GetWorld()->GetTimerManager().SetTimer(
		CoolTimerHandle,
		this,
		&UGearBase::Reset,
		CoolTime,
		false);

	// 状態クラス実行処理
	CurrentGearState->Execute(CurrentGearLevel);

	if (CurrentGearState->GetGearLevelIndex() == kLv4Index)
	{
		OwnerGearComponent->SetIsInvincible(false);
	}
	
	SetAutoAttackEffectVisibility(false);
}

void UGearBase::Update(float DeltaTime)
{
	if (!bIsActive)			{ return; }
	if (!CurrentGearState ||
		!OwnerGearComponent){ return; }

	// 経過時間更新
	ElapsedTime += DeltaTime;

	// 状態クラスの更新
	CurrentGearState->Update(DeltaTime);

	if (ElapsedTime >= SimultaneousActivationCoolTime
		&& !bAllowOtherGearActivation)
	{
		bAllowOtherGearActivation = true;
	}
	
	// 発動時間が終了したら
	// 状態クラスの終了処理を呼び出し、更新を行わない
	if (ElapsedTime >= Duration)
	{
		CurrentGearState->End();
		bIsActive	= false;
		ElapsedTime = 0.0f;
		SetAutoAttackEffectVisibility(true);
		
		if (CurrentGearState->GetGearLevelIndex() == kLv4Index)
		{
			const FTimerHandle& TimerHandle = OwnerGearComponent->GetInvincibilityTimerHandle();
			const bool IsTimerActive = GetWorld()->GetTimerManager().IsTimerActive(TimerHandle);
			if (IsTimerActive)
			{
				OwnerGearComponent->SetIsInvincible(true);
			}
		}
		
	}
}

void UGearBase::ForceStop()
{
	if (!CurrentGearState) { return; }
	CurrentGearState->End();
	bIsActive	= false;
	ElapsedTime = 0.0f;
}

float UGearBase::GetGearDuration(int32 Index) const
{
	if (!GearStatuses.IsValidIndex(Index)) { return 0.f; }
	return GearStatuses[Index].Duration;
}

float UGearBase::GetGearCoolTime(int32 Index) const
{
	if (!GearStatuses.IsValidIndex(Index)) { return 0.f; }
	return GearStatuses[Index].CoolTime;
}

void UGearBase::SetGearDuration(float InDuration, int32 Index)
{
	if (!GearStatuses.IsValidIndex(Index)) { return; }
	GearStatuses[Index].Duration = InDuration;
}

void UGearBase::Reset()
{
	bCanExecute = true;

	const auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }

	const TObjectPtr<UPlayerParameterData> ParameterData
		= SuperGameInstance->GetPlayerParameterData();
	if (!ParameterData)		{ return; }

	ParameterData->ResetSkillCooldown(GearIndex);
	//UE_LOG(LogTemp, Display, TEXT("GearIndex %d"), GearIndex);

}

void UGearBase::SetAutoAttackEffectVisibility(bool bVisibility)
{
	if (!OwnerPlayer) { return; }
	const auto AutoAttack = OwnerPlayer->GetAutoAttack();
	if (!AutoAttack) { return; }
	AutoAttack->SetVisibility(bVisibility);
}


