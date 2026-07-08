
#include "PlayerRuntimeData.h"

#include <Kismet/GameplayStatics.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/UI/PlayerExpUpgradeWidget/PlayerExpUpgradeWidget.h>
#include <ProjectNull/Data/ExpUpgradeDataTable/ExpUpgradeDataTable.h>

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

	// プレイヤーの情報を取得する（0番:1P）
	auto* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!PlayerPawn) { return; }

	if (auto* PlayerBase = Cast<APlayerBase>(PlayerPawn))
	{
		Owner = PlayerBase;
	}

	UpdateStatus();

	// プレイヤーのパラメータデータ取得
	//const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetCharacterParameterData();

	// プレイヤーのHPを更新
	//Health.Current = ParameterData->GetPlayerParameterData()->GetMaxHealth();
}

void UPlayerRuntimeData::AddExperience(float Amount)
{
	Experience.Add(Amount);
	
	// 経験値によるレベルアップ
	while (Experience.Current >= Experience.ExperienceToNextLevel)
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
	Attack.Final = Attack.Base * AttackMultiplier;

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

	/*UE_LOG(LogTemp, Warning, TEXT("hi Total %.0f"), Experience.Total);
	UE_LOG(LogTemp, Warning, TEXT("hi Current %.0f"), Experience.Current);
	UE_LOG(LogTemp, Warning, TEXT("hi ExperienceToNextLevel %.0f"), Experience.ExperienceToNextLevel);
	UE_LOG(LogTemp, Warning, TEXT("hi Level %d"), Level);*/
	//UE_LOG(LogTemp, Warning, TEXT("hi Final %.0f"), Speed.Final);
	
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
	if (!Data.GearLevelSpeedMultiplierArray.IsValidIndex(CurrentGearLevel)) { return; }
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
		return;
	}

	// プレイヤーのパラメータデータ取得
	const TObjectPtr<UPlayerParameterData> ParameterData = Owner->GetSuperGameInstance()->GetPlayerParameterData();

	CalculateExperience(ParameterData->GetExperienceData());
	CalculateFinalSpeed(ParameterData->GetSpeedData(), Owner->GetCurrentGearLevel());
	ApplyMovementSpeed();
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

void UPlayerRuntimeData::UpgradeAttackMultiplier(
	FName Id,
	float InMultiplier)
{
	for (auto& UpgradeState : UpgradeStates)
	{
		if (UpgradeState.UpgradeId == Id)
		{
			if (Id == "0")
			{
				float Multiplier = InMultiplier;

				UE_LOG(LogTemp, Error, TEXT("%f:Multiplier"), Multiplier);

				// 強化画面での攻撃倍率の更新処理
				SetPlayerAttackDamage(Multiplier);
				break;
			}
		}
	}
}