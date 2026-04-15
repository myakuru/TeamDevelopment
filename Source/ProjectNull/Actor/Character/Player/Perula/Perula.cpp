#include "Perula.h"
#include "GameFramework/CharacterMovementComponent.h"

// HpWidgetクラスのインクルード
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>

// コントローラークラスのインクルード
#include <ProjectNull/System/Controller/RobotController/RobotController.h>

APerula::APerula()
{
	if (GetCharacterMovement())
	{
		// 移動方向に合わせてキャラクターを回転させる
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void APerula::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	if (ARobotController* RobotController = Cast<ARobotController>(GetController()))
	{
		PlayerHUDWidget = RobotController->GetPlayerHUD();
	}

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->SetPlayerHp(MaxHp, CurrentHp);
	}
}

void APerula::BeginPlay()
{
	Super::BeginPlay();

	if (PlayerHUDWidget)
	{
		PlayerHUDWidget->SetPlayerHp(MaxHp, CurrentHp);
	}
}

