#include "Perula.h"
#include "GameFramework/CharacterMovementComponent.h"

// HpWidget�N���X�̃C���N���[�h
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>

// �R���g���[���[�N���X�̃C���N���[�h
#include <ProjectNull/System/Controller/RobotController/RobotController.h>

APerula::APerula()
{
	if (GetCharacterMovement())
	{
		// �ړ������ɍ��킹�ăL�����N�^�[���]������
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

