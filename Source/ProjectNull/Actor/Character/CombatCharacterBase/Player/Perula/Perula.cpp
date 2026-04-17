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

}

void APerula::BeginPlay()
{
	Super::BeginPlay();

	
}

