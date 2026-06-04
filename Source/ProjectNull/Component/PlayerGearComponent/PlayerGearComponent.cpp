#include "PlayerGearComponent.h"

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>


UPlayerGearComponent::UPlayerGearComponent():
		OwnerPlayer(nullptr),
		PlayerGears(TArray<UGearBase*>()),
		CurrentGearLevel(1)
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerGearComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerBase>(GetOwner());

	auto SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance) { return; }

	const TObjectPtr<UPlayerParameterData> ParameterData
		= SuperGameInstance->GetPlayerParameterData();
	if (!ParameterData) { return; }

	for (int32 Index = 0; Index < PlayerGears.Num(); ++Index)
	{
		if (!PlayerGears[Index]) { continue; }
		PlayerGears[Index]->Initialize(OwnerPlayer, this);
		PlayerGears[Index]->SetGearIndex(Index);
		//ParameterData->SetSkillCooldownTime(Index, PlayerGears[Index]->GetGearCoolTime(CurrentGearLevel));
	}

}

void UPlayerGearComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGearWidget(DeltaTime);

	for(auto& Gear : PlayerGears) {
		if (!Gear) { continue; }
		Gear->Update(DeltaTime);
	}

	UpdateCollisionByInvincibility();

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
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData()) {
		return;
	}
	if (!CanChangeGear()) { return; }
	const TObjectPtr<UPlayerRuntimeData> RuntimeData = OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData();
	RuntimeData->ResetDataOnGearChange(CurrentGearLevel);
	

	// プレイヤーのパラメータデータ取得
	const TObjectPtr<UPlayerParameterData> ParameterData = OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData();
	RuntimeData->CalculateInvincibilityTime(ParameterData->GetGearData());
	CurrentGearLevel = (CurrentGearLevel % 4 + 1);
	UE_LOG(LogTemp, Warning, TEXT("hi level %d"), CurrentGearLevel);

	//for (int32 Index = 0; Index < PlayerGears.Num(); ++Index)
	//{
	//	const TObjectPtr<const UGearBase> Gear = PlayerGears[Index];
	//	if (!Gear) { continue; }

	//	//UE_LOG(LogTemp, Display, TEXT("RemainTime %.2f"), RemainTime);
	//	ParameterData->SetSkillCooldownTime(Index, Gear->GetGearCoolTime(CurrentGearLevel));
	//}

	OnInvincibilityStart();
}

void UPlayerGearComponent::SetIsInvincible(bool SetFlg)
{
	if (!GetWorld()->GetGameInstance<USuperGameInstance>()
		|| !GetWorld()->GetGameInstance<USuperGameInstance>()->GetPlayerRuntimeData()) { return; }
	const TObjectPtr<UPlayerRuntimeData> RuntimeData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetPlayerRuntimeData();
	RuntimeData->SetIsInvincible(SetFlg);
}

bool UPlayerGearComponent::CanChangeGear() const
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance() || !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()) {
		return false;
	}

	const TObjectPtr<UPlayerRuntimeData> PlayerRuntimeData = OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData();
	return PlayerRuntimeData->CanChangeGear(CurrentGearLevel);
}

void UPlayerGearComponent::OnInvincibilityStart()
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData())
	{
		return;
	}

	const TObjectPtr<UPlayerRuntimeData> RuntimeData = OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData();

	SetIsInvincible(true);

	GetWorld()->GetTimerManager().SetTimer(
		InvincibilityTimerHandle,
		this,
		&UPlayerGearComponent::OnInvincibilityEnd,
		RuntimeData->GetGearData().GearChangeInvincibilityTime,
		false);
}

void UPlayerGearComponent::OnInvincibilityEnd()
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData())
	{
		return;
	}


	if (CurrentGearLevel == kMaxGearLevel) {
		CurrentGearLevel = 1;
		OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()->LevelUp();
	}

	SetIsInvincible(false);
}

void UPlayerGearComponent::UpdateCollisionByInvincibility()
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData()) {
		return;
	}

	const TObjectPtr<UPlayerRuntimeData> RuntimeData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetPlayerRuntimeData();
	const TObjectPtr<UPlayerParameterData> ParameterData = OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData();

	if (!RuntimeData->IsInvincible()) { return; }
	
	const FVector PlayerLocation = OwnerPlayer->GetActorLocation();
	const float InvincibilityCollisionRadiusSquared = ParameterData->GetGearData().InvincibilityCollisionRadiusSquared;
	auto* EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();

	const FRotator	YawRotation = { 0.f, OwnerPlayer->GetControlRotation().Yaw, 0.f };
	const FVector	Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerPlayer->AddMovementInput(Forward);

	if (!EnemyManager) { return; }

	for (const auto& Enemy : EnemyManager->GetEnemyList())
	{
		if (!Enemy) { continue; }

		const float DistSq = FVector::DistSquared(PlayerLocation, Enemy->GetActorLocation());

		if (DistSq <= InvincibilityCollisionRadiusSquared)
		{
			Enemy->SetKnockBackData(PlayerLocation, 3, 1);
			Enemy->SetTakeDamaged();
		}
	}

	DrawDebugSphere(
		GetWorld(),
		OwnerPlayer->GetActorLocation(),
		FMath::Sqrt(InvincibilityCollisionRadiusSquared),
		16,
		FColor::Green,
		false,
		0.1f);

}

void UPlayerGearComponent::UpdateGearWidget(float DeltaTime)
{
	if (!OwnerPlayer || !OwnerPlayer->GetSuperGameInstance()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerRuntimeData()
		|| !OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData()) {
		return;
	}
	TObjectPtr<UPlayerParameterData> ParameterData
		= OwnerPlayer->GetSuperGameInstance()->GetPlayerParameterData();

	for (int32 Index = 0; Index < PlayerGears.Num(); ++Index)
	{
		const TObjectPtr<const UGearBase> Gear = PlayerGears[Index];
		if (!Gear) { continue; }
		const float RemainTime
			= GetWorld()->GetTimerManager().GetTimerRemaining(Gear->GetCoolTimerHandle());
		UE_LOG(LogTemp, Display, TEXT("RemainTime %.2f"),RemainTime);
		ParameterData->UpdateSkillCooldown(Index, RemainTime);
	}

}

