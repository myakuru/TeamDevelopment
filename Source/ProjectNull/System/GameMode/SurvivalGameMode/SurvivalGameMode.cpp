
#include "SurvivalGameMode.h"

#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

// HpWidget�N���X�̃C���N���[�h
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

	// �G�Ǘ��N���X�̏��擾
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	
	// �G�Ǘ��N���X�̍X�V���\�b�h��Ăԁi���t���[���j
	if (enemyManager) {
		UE_LOG(LogTemp, Warning, TEXT("num %d"), enemyManager->GetEnemyNum());
		enemyManager->UpdateEnemies(DeltaTime);
	}
}
