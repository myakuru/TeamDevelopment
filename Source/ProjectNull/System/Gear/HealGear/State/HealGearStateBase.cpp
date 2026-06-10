#include "HealGearStateBase.h"

//ゲームインスタンス
#include<ProjectNull/GameInstance/SuperGameInstance.h>
//プレイヤーランタイムデータ
#include<ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

UHealGearStateBase::UHealGearStateBase()
{
}

void UHealGearStateBase::Execute(int32 CurrentGearLevel)
{
	UGearStateBase::Execute(CurrentGearLevel);
	Heal();
}

void UHealGearStateBase::Update(float DeltaTime)
{
}

void UHealGearStateBase::Heal()
{
	TObjectPtr<UPlayerRuntimeData> playerRuntimeData = GetWorld()->GetGameInstance<USuperGameInstance>()->GetPlayerRuntimeData();

	if (playerRuntimeData)
	{
		//回復量
		float max = playerRuntimeData->GetMaxHealth();
		float now = playerRuntimeData->GetHealth();
		UE_LOG(LogTemp, Error, TEXT("NowHp / MaxHp : %f / %f"), now, max);

		playerRuntimeData->AddHealth(HealAmount);
		UE_LOG(LogTemp, Error, TEXT("HealAmount : %f"), HealAmount);

		now = playerRuntimeData->GetHealth();
		UE_LOG(LogTemp, Error, TEXT("NowHp / MaxHp : %f / %f"), now, max);
	}
}
