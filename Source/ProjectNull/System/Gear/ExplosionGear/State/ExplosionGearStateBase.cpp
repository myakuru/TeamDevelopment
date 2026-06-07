// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearStateBase.h"
#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull\Actor\Explosion\ExplosionGearSkill.h>
#include "Kismet\GameplayStatics.h"

UExplosionGearStateBase::UExplosionGearStateBase()
{
}

void UExplosionGearStateBase::Execute(int32 CurrentGearLevel)
{
	Super::Execute(CurrentGearLevel);

	if (Owner) {
		Owner->SetBlocksMovement(true);
	}

}

void UExplosionGearStateBase::Update(float DeltaTime)
{
	Super::Update(DeltaTime);
}

void UExplosionGearStateBase::SpawnExplosions()
{
	if (!ExplosionClass)return;

	for (const FExplosionSpawnData& data : ExplosionDatas) {
		FVector spawnLocation = Player->GetActorTransform().TransformPosition(data.Offset);
		AExplosionGearSkill* Explosion =
			GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
				ExplosionClass,
				FTransform(spawnLocation)
			);

		FExplosionData initializeData;
		initializeData.Damage = data.Damage;
		initializeData.Delay = data.Delay;
		initializeData.IgnitionDelay = data.IgnitionDelay;
		initializeData.Scale = data.Scale;
		Explosion->Initialize(initializeData);

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			FTransform(spawnLocation)
		);

	}
}

void UExplosionGearStateBase::SpawnExplosion(const FExplosionSpawnData& ExplosionData)
{

	if (!ExplosionClass)return;

	FVector spawnLocation = Player->GetActorTransform().TransformPosition(ExplosionData.Offset);
	AExplosionGearSkill* Explosion =
		GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
			ExplosionClass,
			FTransform(spawnLocation)
		);

	FExplosionData initializeData;
	initializeData.Damage = ExplosionData.Damage;
	initializeData.Delay = ExplosionData.Delay;
	initializeData.IgnitionDelay = ExplosionData.IgnitionDelay;
	initializeData.Scale = ExplosionData.Scale;
	Explosion->Initialize(initializeData);

	UGameplayStatics::FinishSpawningActor(
		Explosion,
		FTransform(spawnLocation)
	);

}

void UExplosionGearStateBase::SpawnExplosionsInCircle(const FExplosionSpawnData& ExplosionData,float CircleRadius, int32 Count, float Interval)
{
	if (!ExplosionClass)return;
	for (int i = 0; i < Count; i++) {

		float angle = (360.0f / Count) * i;

		FVector offset = FRotator(0.0f, angle, 0.0f).RotateVector(FVector(CircleRadius, 0.0f, 0.0f));

		FVector spawnLocation = Player->GetActorTransform().TransformPosition(offset);

		AExplosionGearSkill* Explosion =
			GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
				ExplosionClass,
				FTransform(spawnLocation)
			);

		FExplosionData initializeData;
		initializeData.Damage = ExplosionData.Damage;
		initializeData.Delay = ExplosionData.Delay + Interval * i;
		initializeData.IgnitionDelay = ExplosionData.IgnitionDelay;
		initializeData.Scale = ExplosionData.Scale;
		Explosion->Initialize(initializeData);

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			FTransform(spawnLocation)
		);

	}
}
