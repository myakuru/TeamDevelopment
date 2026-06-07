
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

	OpeningCameraActor = Cast<AMyCineCameraActor>(
		UGameplayStatics::GetActorOfClass(GetWorld(), AMyCineCameraActor::StaticClass()));

	if (!OpeningCameraActor) return;

	APlayerController* PC = GetWorld()->GetFirstPlayerController();

	if (ARobotController* RC = Cast<ARobotController>(PC))
	{
		RC->SetCanReceiveInput(false);
	}

	if (PC)
	{
		PC->SetViewTargetWithBlend(OpeningCameraActor, TargetBlendSpeed);
	}

	OpeningCameraActor->OnCutsceneFinished.AddDynamic(this, &ASurvivalGameMode::OnCutsceneFinished);
	OpeningCameraActor->PlayOpeningCutscene();
}

void ASurvivalGameMode::OnCutsceneFinished()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerBase* Player = PC ? Cast<APlayerBase>(PC->GetPawn()) : nullptr;

	if (ARobotController* RC = Cast<ARobotController>(PC))
	{
		RC->SetCanReceiveInput(true);
	}

	if (PC && Player)
	{
		PC->SetViewTargetWithBlend(Player, TargetBlendSpeed);
	}
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
