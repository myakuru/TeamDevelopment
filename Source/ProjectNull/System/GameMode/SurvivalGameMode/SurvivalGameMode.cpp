
#include "SurvivalGameMode.h"

#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
// HpWidgetクラスのインクルード
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>


ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// 敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
	// 敵管理クラスの更新メソッドを呼ぶ（毎フレーム）
	if (enemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("num %d"), enemyManager->GetEnemyNum());
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
