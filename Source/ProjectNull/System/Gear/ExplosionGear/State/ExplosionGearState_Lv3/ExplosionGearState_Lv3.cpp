// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearState_Lv3.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

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

}

void UExplosionGearState_Lv3::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}
