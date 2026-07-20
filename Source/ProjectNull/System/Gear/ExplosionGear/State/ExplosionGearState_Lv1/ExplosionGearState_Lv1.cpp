#include "ExplosionGearState_Lv1.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull\Actor\Explosion\ExplosionGearSkill.h>
#include "Kismet\GameplayStatics.h"

UExplosionGearState_Lv1::UExplosionGearState_Lv1()
{
}

void UExplosionGearState_Lv1::Execute(int32 CurrentGearLevel)
{
	Super::Execute(CurrentGearLevel);

	SpawnExplosionsInCircle(CircularExplosionData, ExplosionSpawnRadius, ExplosionCount);
	
	// 効果音
	if (GearSESound.IsValidIndex(SEIndex::FirecrackersSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::FirecrackersSESoundIndex]);
	}
	if (GearSESound.IsValidIndex(SEIndex::LowFlameSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::LowFlameSESoundIndex]);
	}
}

void UExplosionGearState_Lv1::Update(float DeltaTime)
{
}
