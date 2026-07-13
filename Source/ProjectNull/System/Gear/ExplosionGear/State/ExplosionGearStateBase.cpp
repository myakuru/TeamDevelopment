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
	if (!ExplosionClass ||
		!Player)return;

	for (const FExplosionSpawnData& data : ExplosionDatas) {
		FTransform SpawnTransform;
		FVector spawnLocation = Player->GetActorTransform().TransformPosition(data.Offset);

		const auto RootComp = Player->GetGroundAlignmentRootComponent();
		if (!RootComp) { return; }
		const FQuat SpawnQuat = RootComp->GetComponentQuat();
		
		SpawnTransform.SetLocation(spawnLocation);
		SpawnTransform.SetRotation(SpawnQuat);
		AExplosionGearSkill* Explosion =
			GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
				ExplosionClass,
				SpawnTransform
			);

		FExplosionData initializeData;
		initializeData.Damage = data.Damage;
		initializeData.Delay = data.Delay;
		initializeData.IgnitionDelay = data.IgnitionDelay;
		initializeData.Scale = data.Scale;
		Explosion->Initialize(initializeData);
		Explosion->StartExplosionSequence();

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			SpawnTransform
		);

	}
}

void UExplosionGearStateBase::SpawnExplosion(const FExplosionSpawnData& ExplosionData)
{

	if (!ExplosionClass)return;
	
	FTransform SpawnTransform;
	FVector spawnLocation = Player->GetActorTransform().TransformPosition(ExplosionData.Offset);

	const auto RootComp = Player->GetGroundAlignmentRootComponent();
	if (!RootComp) { return; }
	const FQuat SpawnQuat = RootComp->GetComponentQuat();
		
	SpawnTransform.SetLocation(spawnLocation);
	SpawnTransform.SetRotation(SpawnQuat);
	
	AExplosionGearSkill* Explosion =
		GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
			ExplosionClass,
			SpawnTransform
		);

	FExplosionData initializeData;
	initializeData.Damage = ExplosionData.Damage;
	initializeData.Delay = ExplosionData.Delay;
	initializeData.IgnitionDelay = ExplosionData.IgnitionDelay;
	initializeData.Scale = ExplosionData.Scale;
	Explosion->Initialize(initializeData);
	Explosion->StartExplosionSequence();

	UGameplayStatics::FinishSpawningActor(
		Explosion,
		SpawnTransform
	);

}

void UExplosionGearStateBase::SpawnExplosionsInCircle(
	const FExplosionSpawnData& ExplosionData,
	float CircleRadius,
	int32 Count,
	float Interval)
{
	if (!ExplosionClass ||
		!Player) { return; }
	
	for (int i = 0; i < Count; i++) {

		float angle = (360.0f / Count) * i;

		const auto RootComp = Player->GetGroundAlignmentRootComponent();
		if (!RootComp) { return; }
		FRotator SpawnRotator = RootComp->GetComponentQuat().Rotator();
		FVector Forward = Player->GetActorForwardVector();
		//SpawnRotator.Yaw = angle;
		FVector offset2 = FRotator(0.f,angle,0.f).RotateVector(FVector(CircleRadius, 0.0f, 0.0f));
		FVector offset1 = SpawnRotator.RotateVector(offset2);
		
		FTransform SpawnTransform;
		FVector spawnLocation = Player->GetActorTransform().TransformPosition(offset1);

		SpawnTransform.SetLocation(spawnLocation);
		//SpawnTransform.SetRotation(SpawnQuat);

		AExplosionGearSkill* Explosion =
			GetWorld()->SpawnActorDeferred<AExplosionGearSkill>(
				ExplosionClass,
				SpawnTransform
			);

		FExplosionData initializeData;
		initializeData.Damage = ExplosionData.Damage;
		initializeData.Delay = ExplosionData.Delay + Interval * i;
		initializeData.IgnitionDelay = ExplosionData.IgnitionDelay;
		initializeData.Scale = ExplosionData.Scale;
		Explosion->Initialize(initializeData);
		Explosion->StartExplosionSequence();

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			SpawnTransform
		);

	}
}
