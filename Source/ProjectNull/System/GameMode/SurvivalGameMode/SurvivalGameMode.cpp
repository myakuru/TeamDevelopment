
#include "SurvivalGameMode.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/Actor/MyCineCameraActor/MyCineCameraActor.h>

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

	OpeningCameraActor->PlayOpeningCutscene();
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OpeningCameraActor)
	{
		OpeningCameraActor->UpdateActorRelativeLocation();
	}

	// 敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
	// 敵管理クラスの更新メソッドを呼ぶ（毎フレーム）
	if (enemyManager) {
		//(LogTemp, Warning, TEXT("num %d"), enemyManager->GetEnemyNum());
		enemyManager->UpdateEnemies(DeltaTime);
	}

	/** アイテム管理クラスの情報取得*/
	UItemManagerSubsystem* itemManager = GetWorld()->GetSubsystem<UItemManagerSubsystem>();

	/** 更新*/
	if (itemManager)
	{
		itemManager->UpdateItemManagers(DeltaTime);
	}
}
