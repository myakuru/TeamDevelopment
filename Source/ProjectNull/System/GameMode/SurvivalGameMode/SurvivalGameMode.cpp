
#include "SurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/Actor/MyCineCameraActor/MyCineCameraActor.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalGameMode::OnCutsceneFinished()
{
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
