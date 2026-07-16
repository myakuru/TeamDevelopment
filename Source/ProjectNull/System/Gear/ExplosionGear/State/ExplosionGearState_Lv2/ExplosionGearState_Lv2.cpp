#include "ExplosionGearState_Lv2.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

UExplosionGearState_Lv2::UExplosionGearState_Lv2()
{
}

void UExplosionGearState_Lv2::Execute(int32 CurrentGearLevel)
{
	Super::Execute(CurrentGearLevel);

	SpawnExplosions();

	for (const FCircleSpawnData& data : CircleSpawnDatas) {
		SpawnExplosionsInCircle(data.ExplosionData, data.Radius, data.Count);
	}
	
	//効果音
	if (GearSESound.IsValidIndex(SEIndex::FirecrackersSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::FirecrackersSESoundIndex]);
	}
	if (GearSESound.IsValidIndex(SEIndex::StrongFlameSESoundIndex))
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(GearSESound[SEIndex::StrongFlameSESoundIndex]);
	}
}

void UExplosionGearState_Lv2::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}
