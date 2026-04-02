// Fill out your copyright notice in the Description page of Project Settings.


#include "SurvivalGameMode.h"

#include "../../Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h"

ASurvivalGameMode::ASurvivalGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASurvivalGameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//　敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
	//　敵管理クラスの更新メソッドを呼ぶ（毎フレーム）
	if (enemyManager) {
		//UE_LOG(LogTemp, Warning, TEXT("num %d"), enemyManager->GetEnemyNum());
		enemyManager->UpdateEnemies(DeltaTime);
	}
}
