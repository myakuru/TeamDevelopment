// Fill out your copyright notice in the Description page of Project Settings.


#include "ExplosionGearStateBase.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull\Actor\Explosion\ExplosionGearSkill.h>
#include "Kismet\GameplayStatics.h"

UExplosionGearStateBase::UExplosionGearStateBase()
{
}

void UExplosionGearStateBase::Execute(int32 CurrentGearLevel)
{
	UE_LOG(LogTemp, Warning, TEXT("ExplosionGearStateBase Exucute"));
	Super::Execute(CurrentGearLevel);

	SpawnExplosions();
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
		initializeData.Radius = data.Radius;
		Explosion->Initialize(initializeData);

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			FTransform(spawnLocation)
		);

	}
}
