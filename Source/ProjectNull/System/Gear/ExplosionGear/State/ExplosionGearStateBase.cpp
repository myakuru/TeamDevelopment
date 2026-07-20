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

	const auto rootComp = Player->GetGroundAlignmentRootComponent();
	if (!rootComp) { return; }
	FRotator groundRot = rootComp->GetComponentQuat().Rotator();
	FRotator fowordRot = Player->GetActorForwardVector().Rotation();

	for (const FExplosionSpawnData& data : ExplosionDatas) {
		
		FTransform SpawnTransform;
		FVector offset = groundRot.RotateVector(fowordRot.RotateVector(data.Offset));
		FVector spawnLocation = Player->GetActorLocation() + offset;

		SpawnTransform.SetLocation(spawnLocation);
		SpawnTransform.SetRotation(rootComp->GetComponentQuat());
		
		SpawnTransform.SetLocation(spawnLocation);
		SpawnTransform.SetRotation(rootComp->GetComponentQuat());
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
	
	const auto rootComp = Player->GetGroundAlignmentRootComponent();
	if (!rootComp) { return; }
	FRotator groundRot = rootComp->GetComponentQuat().Rotator();
	FRotator fowordRot = Player->GetActorForwardVector().Rotation();

	FTransform SpawnTransform;
	FVector offset = groundRot.RotateVector(fowordRot.RotateVector(ExplosionData.Offset));
	FVector spawnLocation = Player->GetActorLocation() + offset;

	SpawnTransform.SetLocation(spawnLocation);
	SpawnTransform.SetRotation(rootComp->GetComponentQuat());
	
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

		const auto rootComp = Player->GetGroundAlignmentRootComponent();
		if (!rootComp) { return; }
		FRotator groundRot = rootComp->GetComponentQuat().Rotator();
		FRotator fowordRot = Player->GetActorForwardVector().Rotation();

		FVector offset = groundRot.RotateVector(fowordRot.RotateVector(FRotator(0.f, angle, 0.f).RotateVector(FVector(CircleRadius, 0.0f, 0.0f))));
		
		FTransform SpawnTransform;
		FVector spawnLocation = Player->GetActorLocation() + offset;

		SpawnTransform.SetLocation(spawnLocation);
		SpawnTransform.SetRotation(rootComp->GetComponentQuat());

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
		
		if (!Explosion) { return; }
		Explosion->Initialize(initializeData);
		Explosion->StartExplosionSequence();

		UGameplayStatics::FinishSpawningActor(
			Explosion,
			SpawnTransform
		);

	}
}
