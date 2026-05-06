
#include "PlayerRuntimeData.h"

#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/CharacterParameterData.h>

UPlayerRuntimeData::UPlayerRuntimeData():
	Level(1)
{

}

void UPlayerRuntimeData::Initialize()
{
	UpdateStatus();
}

void UPlayerRuntimeData::AddExperience(float Amount)
{
	Experience.Add(Amount);
	
	/* 経験値によるレベルアップ */
	while (Experience.CanLevelUp())
	{
		Experience.Current -= Experience.ExperienceToNextLevel;

		LevelUp();
	}
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
}

void UPlayerRuntimeData::CalculateExperience(const FExperienceParameterData& Data)
{
	Experience.CalculateExperienceToNextLevel(Data.BaseExperienceToNextLevel, Data.ExperienceToNextLevelIncreasePerLevel, Level);
}

void UPlayerRuntimeData::CalculateFinalSpeed(const FSpeedParameterData& Data, int32 CurrentGearLevel)
{
	if (!Data.GearLevelSpeedMultiplierArray.IsValidIndex(CurrentGearLevel)) { return; }
	const float gearLevelSpeedMultiplier = Data.GearLevelSpeedMultiplierArray[CurrentGearLevel];
	Speed.Final = (Data.Base + Level * Data.ScalePerLevelSpeed) * gearLevelSpeedMultiplier;
}

void UPlayerRuntimeData::UpdateStatus()
{
	if (!Owner || !Owner->GetSuperGameInstance() || !Owner->GetSuperGameInstance()->GetCharacterParameterData()) {
		return;
	}
	/* プレイヤーのパラメータデータ取得 */
	const TObjectPtr<UCharacterParameterData> parameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	CalculateExperience(parameterData->GetExperienceData());
	CalculateFinalSpeed(parameterData->GetSpeedData(), Owner->GetCurrentGearLevel());
	ApplyMovementSpeed();
}

