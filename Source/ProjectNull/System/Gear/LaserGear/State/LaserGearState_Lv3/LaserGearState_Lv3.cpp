
#include "LaserGearState_Lv3.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Combat/Attack/CrossLaserAttack/CrossLaserAttack.h>

ULaserGearState_Lv3::ULaserGearState_Lv3()
{

}

void ULaserGearState_Lv3::Initialize(
	APlayerBase* InPlayer,
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	ULaserGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner);

	if (!CrossLaserAttack) { return; }
	CrossLaserAttack->Initialize(InPlayer);
}

void ULaserGearState_Lv3::Execute(int32 CurrentGearLevel)
{
	ULaserGearStateBase::Execute(CurrentGearLevel);
	if (!CrossLaserAttack) { return; }
	CrossLaserAttack->Execute();
	
	//効果音
	if (GearSESound.IsValidIndex(SEIndex::CrossLaserSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::CrossLaserSESoundIndex]);
	}
}

void ULaserGearState_Lv3::Update(float DeltaTime)
{
	ULaserGearStateBase::Update(DeltaTime);
	if (!CrossLaserAttack) { return; }
	CrossLaserAttack->Update(DeltaTime);
}

void ULaserGearState_Lv3::End()
{
	ULaserGearStateBase::End();
	if (!CrossLaserAttack) { return; }
	CrossLaserAttack->End();
}
