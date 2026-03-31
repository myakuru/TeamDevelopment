#include "Perula.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
APerula::APerula()
{
	if (GetCharacterMovement())
	{
		// 移動方向に合わせてキャラクターを回転させる
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

