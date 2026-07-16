
#include "SurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/UI/InGame/HitDamageWidget/DamageNumberActor/DamageNumberActor.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/DamageNumberPoolSubsystem/DamageNumberPoolSubsystem.h>
#include <ProjectNull/System/WorldSystem/EnemySpawner/EnemySpawner.h>

ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (UDamageNumberPoolSubsystem* Pool = GetWorld()->GetSubsystem<UDamageNumberPoolSubsystem>())
	{
		Pool->InitializePool(DamageNumberActorClass, DamageNumberPoolSize);
	}
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetWorld()->GetFirstPlayerController()->WasInputKeyJustPressed(EKeys::Y))
	{
		AActor* FoundActor = UGameplayStatics::GetActorOfClass(
			GetWorld(), AEnemySpawner::StaticClass());

		AEnemySpawner* EnemySpawner = Cast<AEnemySpawner>(FoundActor);

		if (IsValid(EnemySpawner))
		{
			EnemySpawner->SetFinalPhase();
		}

	}

	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (enemyManager)
	{
		enemyManager->UpdateEnemies(DeltaTime);
	}

	UItemManagerSubsystem* itemManager = GetWorld()->GetSubsystem<UItemManagerSubsystem>();
	if (itemManager)
	{
		itemManager->UpdateItemManagers(DeltaTime);
	}
}
