
#include "PlayerRuntimeData.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>

UPlayerRuntimeData::UPlayerRuntimeData() :
	Owner(nullptr),
	Level(1)
{
	CachedExpUpgradeTable = LoadObject<UDataTable>(nullptr, TEXT("/Game/DataTable/DT_ExpUpgrade"));
	if (!CachedExpUpgradeTable)
	{
		UE_LOG(LogTemp, Warning, TEXT("DT_ExpUpgrade がロードできません"));
	}

	// 要素数分の初期化
	for (const FName& RowName : CachedExpUpgradeTable->GetRowNames())
	{
		UpgradeStates.Add({ RowName, "0"});
	}
}

void UPlayerRuntimeData::Initialize()
{

	//UpdateStatus();

	// プレイヤーのパラメータデータ取得
	//const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	// プレイヤーのHPを更新
	//Health.Current = ParameterData->GetPlayerParameterData()->GetMaxHealth();
}

void UPlayerRuntimeData::AddExperience(float Amount)
{
	//UE_LOG(LogTemp, Warning, TEXT("hi Amount %.0f"), Amount);
	Experience.Add(Amount);
	
	// 変更があれば、経験値のバーのUIが更新される
	OnExperienceChanged.Broadcast(Experience.Current, Experience.ExperienceToNextLevel);
	
	// 経験値によるレベルアップ
	while (Experience.Current >= Experience.ExperienceToNextLevel)
	{
		Experience.Current -= Experience.ExperienceToNextLevel;
		
		OnExperienceChanged.Broadcast(1.0f, 1.0f);
		
		LevelUp();
	}
}

void UPlayerRuntimeData::AddGearEnergy(float Amount)
{
	Gear.GearEnergy += Amount;
	//UE_LOG(LogTemp, Warning, TEXT("hi GearEnergy %.0f"), Gear.GearEnergy);
	OnGearEnergyChanged.Broadcast(Gear.GearEnergy);
}

void UPlayerRuntimeData::SubtractHealth(float Amount)
{
	Health.SetCurrent(Health.Current - Amount);
	OnHealthChanged.Broadcast(Health.Current, Health.Max);
}

bool UPlayerRuntimeData::CanChangeGear(int32 CurrentGearLevel)
{
	return Gear.CanChangeGear(CurrentGearLevel);
}

void UPlayerRuntimeData::ResetDataOnGearChange(int32 CurrentGearLevel)
{
	Gear.ResetDataOnGearChange(CurrentGearLevel);
}

void UPlayerRuntimeData::ApplyMovementSpeed()
{
	if (!Owner || !Owner->GetCharacterMovement()) { return; }
	Owner->GetCharacterMovement()->MaxWalkSpeed = Speed.Final;
}

float UPlayerRuntimeData::GetPlayerAttackDamage()
{
	// プレイヤーの攻撃力を計算するロジックをここに実装
	Attack.Final = Attack.Base * GetEffectMultiplier(EUpgradeEffectType::AttackDamage);

	//UE_LOG(LogTemp, Error, TEXT("%f:Attack.Final"), Attack.Final);

	return Attack.Final;
}

void UPlayerRuntimeData::LevelUp()
{
	Level++;

	UpdateStatus();

	if (!RobotController)
	{
		RobotController = Cast<ARobotController>(UGameplayStatics::GetPlayerController(this, 0));
	}

	if (RobotController)
	{
		RobotController->OpenPlayerExpUpgradeWidget();
	}

	UE_LOG(LogTemp, Warning, TEXT("hi Total %.0f"), Experience.Total);
	UE_LOG(LogTemp, Warning, TEXT("hi Current %.0f"), Experience.Current);
	UE_LOG(LogTemp, Warning, TEXT("hi ExperienceToNextLevel %.0f"), Experience.ExperienceToNextLevel);
	UE_LOG(LogTemp, Warning, TEXT("hi Level %d"), Level);
	UE_LOG(LogTemp, Warning, TEXT("hi Final %.0f"), Speed.Final);
	
}

void UPlayerRuntimeData::UpdateHealth(float NewHealth)
{
	Health.SetCurrent(NewHealth);

	OnHealthChanged.Broadcast(Health.Current, Health.Max);
}

void UPlayerRuntimeData::CalculateExperience(const FExperienceParameterData& Data)
{
	Experience.CalculateExperienceToNextLevel(Data.BaseExperienceToNextLevel, Data.ExperienceToNextLevelIncreasePerLevel, Level);
}

void UPlayerRuntimeData::CalculateFinalSpeed(
	const FSpeedParameterData& Data,
	int32 CurrentGearLevel)
{
	if (!Data.GearLevelSpeedMultiplierArray.IsValidIndex(--CurrentGearLevel)) { return; }
	
	const float GearLevelSpeedMultiplier = Data.GearLevelSpeedMultiplierArray[CurrentGearLevel];
	Speed.Final = (Data.Base + Level * Data.ScalePerLevelSpeed) * GearLevelSpeedMultiplier;
}

void UPlayerRuntimeData::CalculateInvincibilityTime(const FGearParameterData& Data)
{	
	Gear.CalculateInvincibilityTime(Data.BaseInvincibilityTime,Data.ExtraInvincibilityTimePerExcessRatio);
}

void UPlayerRuntimeData::UpdateStatus()
{
	if (!Owner || !Owner->GetSuperGameInstance() || !Owner->GetSuperGameInstance()->GetPlayerParameterData()) {
		//UE_LOG(LogTemp, Warning, TEXT("hi 止まって"));
		return;
	}

	// プレイヤーのパラメータデータ取得
	const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetPlayerParameterData();

	CalculateExperience(ParameterData->GetExperienceData());
	CalculateFinalSpeed(ParameterData->GetSpeedData(), Owner->GetCurrentGearLevel());
	ApplyMovementSpeed();
}

TArray<FValidUpgradeInfo> UPlayerRuntimeData::BuildUpgradeChoices(int32 Count)
{
	TArray<FValidUpgradeInfo> Result;
	if (!CachedExpUpgradeTable) { return Result; }

	// 行名を取得して重複なくランダム抽選する
	TArray<FName> RowNames = CachedExpUpgradeTable->GetRowNames();
	if (RowNames.Num() < Count) { return Result; }

	for (int32 i = 0; i < Count; ++i)
	{
		const int32 Index = FMath::RandRange(0, RowNames.Num() - 1);
		const FName RowName = RowNames[Index];
		RowNames.RemoveAt(Index);

		// 現在レベルに対応する説明文がなければ候補から除外する
		const FExpUpgradeLevelData* LevelData = FindCurrentLevelData(RowName);
		if (!LevelData) { continue; }

		FValidUpgradeInfo Info;
		Info.RowName = RowName;
		Info.Description = LevelData->Description;
		Info.CurrentLevel = GetUpgradeLevel(RowName);
		Result.Add(Info);
	}

	return Result;
}

void UPlayerRuntimeData::ApplySelectedUpgrade(FName Id)
{
	// レベルを進める前に、いま提示している（現在レベルの）効果データを取得する
	if (const FExpUpgradeLevelData* LevelData = FindCurrentLevelData(Id))
	{
		ApplyUpgradeEffect(LevelData->EffectType, LevelData->AttackMultiplier);
	}

	// 強化レベルを1段進める
	UpdateUpgradeStates(Id);
}

void UPlayerRuntimeData::ApplyUpgradeEffect(EUpgradeEffectType Type, float Value)
{
	// 効果種別ごとに現在倍率を保持する。効果が増えても分岐は増えない
	EffectMultipliers.FindOrAdd(Type) = Value;
}

float UPlayerRuntimeData::GetEffectMultiplier(EUpgradeEffectType Type) const
{
	const float* Found = EffectMultipliers.Find(Type);
	return Found ? *Found : 1.0f;
}

const FExpUpgradeLevelData* UPlayerRuntimeData::FindCurrentLevelData(FName Id) const
{
	if (!CachedExpUpgradeTable) { return nullptr; }

	const FExpUpgradeRow* Row = CachedExpUpgradeTable->FindRow<FExpUpgradeRow>(Id, TEXT(""));
	if (!Row) { return nullptr; }

	const int32 LevelIndex = FCString::Atoi(*GetUpgradeLevel(Id).ToString());
	if (!Row->UpgradeLevels.IsValidIndex(LevelIndex)) { return nullptr; }

	return &Row->UpgradeLevels[LevelIndex];
}

void UPlayerRuntimeData::UpdateUpgradeStates(FName Id)
{
	for(auto& UpgradeState : UpgradeStates)
	{
		if (UpgradeState.UpgradeId == Id)
		{
			int32 CurrentLevel = FCString::Atoi(*UpgradeState.Level.ToString());
			CurrentLevel++;
			UpgradeState.Level = FName(*FString::FromInt(CurrentLevel));
			break;
		}
	}
}

FName UPlayerRuntimeData::GetUpgradeLevel(FName Id) const
{
	for(const auto& UpgradeState : UpgradeStates)
	{
		if (UpgradeState.UpgradeId == Id)
		{
			return UpgradeState.Level;
		}
	}
	return "null";
}