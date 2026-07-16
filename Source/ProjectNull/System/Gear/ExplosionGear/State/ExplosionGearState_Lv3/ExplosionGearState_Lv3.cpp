#include "ExplosionGearState_Lv3.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

UExplosionGearState_Lv3::UExplosionGearState_Lv3()
{
}

void UExplosionGearState_Lv3::Execute(int32 CurrentGearLevel)
{

	Super::Execute(CurrentGearLevel);

	for (const FCircleSpawnData& data : CircleSpawnDatas) {
		SpawnExplosionsInCircle(data.ExplosionData,data.Radius,data.Count);
	}
	
	SpawnExplosion(FirstExplosionData);

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this]
		{
			if (!Player) { return; }

			FVector LastInput = Player->GetLastMovementInputVector();

			const FVector velocity = { 0.0,0.0,JumpPow };
			Player->LaunchCharacter(velocity, true, true);
		}
	);

	GetWorld()->GetTimerManager().SetTimer(
		FirstExplosionTimerHandle,
		timerDelegate,
		FirstExplosionData.Delay + FirstExplosionData.IgnitionDelay,
		false
	);

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

void UExplosionGearState_Lv3::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}
