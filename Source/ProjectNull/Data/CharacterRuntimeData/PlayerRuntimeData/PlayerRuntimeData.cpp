
#include "PlayerRuntimeData.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>

UPlayerRuntimeData::UPlayerRuntimeData():
	Owner(nullptr),
	Level(1)
{
}

void UPlayerRuntimeData::Initialize()
{
	UpdateStatus();

	// プレイヤーの情報を取得する（0番:1P）
	auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) { return; }

	if (auto* PlayerBase = Cast<APlayerBase>(PlayerPawn)) {
		Owner = PlayerBase;
	}

	// プレイヤーのパラメータデータ取得
	//const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	// プレイヤーのHPを更新
	//Health.Current = ParameterData->GetPlayerParameterData()->GetMaxHealth();
}

void UPlayerRuntimeData::AddExperience(float Amount)
{
	Experience.Add(Amount);
	
	// 経験値によるレベルアップ
	while (Experience.CanLevelUp())
	{
		Experience.Current -= Experience.ExperienceToNextLevel;

		LevelUp();
	}

	// 変更があれば、経験値のバーのUIが更新される
	OnExperienceChanged.Broadcast(Experience.Current, Experience.ExperienceToNextLevel);
}

void UPlayerRuntimeData::AddGearEnergy(float Amount)
{
	Gear.GearEnergy += Amount;
}

bool UPlayerRuntimeData::CanChangeGear(int32 CurrentGearLevel)
{
	return Gear.CanChangeGear(CurrentGearLevel);
}

void UPlayerRuntimeData::ApplyMovementSpeed()
{
	if (!Owner || !Owner->GetCharacterMovement()) { return; }
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed.Final;
}

void UPlayerRuntimeData::LevelUp()
{
	Level++;

	UpdateStatus();
	/*UE_LOG(LogTemp, Warning, TEXT("hi Total %.0f"), Experience.Total);
	UE_LOG(LogTemp, Warning, TEXT("hi Current %.0f"), Experience.Current);
	UE_LOG(LogTemp, Warning, TEXT("hi ExperienceToNextLevel %.0f"), Experience.ExperienceToNextLevel);
	UE_LOG(LogTemp, Warning, TEXT("hi Level %d"), Level);*/
	//UE_LOG(LogTemp, Warning, TEXT("hi Final %.0f"), Speed.Final);
	
}

void UPlayerRuntimeData::UpdateHealth(float NewHealth)
{
	Health.Current = FMath::Clamp(NewHealth, 0.0f, Health.Max);

	OnHealthChanged.Broadcast(Health.Current, Health.Max);
}

void UPlayerRuntimeData::CalculateExperience(const FExperienceParameterData& Data)
{
	Experience.CalculateExperienceToNextLevel(Data.BaseExperienceToNextLevel, Data.ExperienceToNextLevelIncreasePerLevel, Level);
}

void UPlayerRuntimeData::CalculateFinalSpeed(const FSpeedParameterData& Data, int32 CurrentGearLevel)
{
	if (!Data.GearLevelSpeedMultiplierArray.IsValidIndex(CurrentGearLevel)) { return; }
	const float GearLevelSpeedMultiplier = Data.GearLevelSpeedMultiplierArray[CurrentGearLevel];
	Speed.Final = (Data.Base + Level * Data.ScalePerLevelSpeed) * GearLevelSpeedMultiplier;
}

void UPlayerRuntimeData::UpdateStatus()
{
	if (!Owner || !Owner->GetSuperGameInstance() || !Owner->GetSuperGameInstance()->GetCharacterParameterData()) {
		return;
	}

	// プレイヤーのパラメータデータ取得
	const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	CalculateExperience(ParameterData->GetExperienceData());
	CalculateFinalSpeed(ParameterData->GetSpeedData(), Owner->GetCurrentGearLevel());
	ApplyMovementSpeed();
}

