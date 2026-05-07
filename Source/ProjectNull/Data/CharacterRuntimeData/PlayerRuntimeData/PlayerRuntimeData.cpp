
#include "PlayerRuntimeData.h"

#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/CharacterParameterData.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>

UPlayerRuntimeData::UPlayerRuntimeData():
	Owner(nullptr),
	Level(1)
{
}

void UPlayerRuntimeData::Initialize()
{
	UpdateStatus();

	// プレイヤーのパラメータデータ取得
	const TObjectPtr<UCharacterParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	// プレイヤーのHPを更新
	Health.Current = ParameterData->GetPlayerParameterData()->GetMaxHealth();
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

void UPlayerRuntimeData::ApplyMovementSpeed()
{
	if (!Owner->GetCharacterMovement()) { return; }
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

	/* プレイヤーのパラメータデータ取得 */
	const TObjectPtr<UCharacterParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	CalculateExperience(ParameterData->GetExperienceData());
	CalculateFinalSpeed(ParameterData->GetSpeedData(), Owner->GetCurrentGearLevel());
	ApplyMovementSpeed();
}

