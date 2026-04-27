// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDirection_MiniMap.h"
#include "Components/Image.h"

// �v���C���[�̃N���X�̃w�b�_�[�t�@�C����C���N���[�h
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include "Kismet/GameplayStatics.h"

void UPlayerDirection_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();
	// NativeTick��L����
	SetIsFocusable(false);
}

void UPlayerDirection_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// �v���C���[�̊p�x��擾
	if (APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		const float PlayerYaw = Player->GetActorRotation().Yaw;

		// �v���C���[�̕�����X�V
		UpdatePlayerDirection(PlayerYaw);
	}
}

void UPlayerDirection_MiniMap::UpdatePlayerDirection(float PlayerYaw)
{
	if (PlayerDirection)
	{
		// Image��RenderTransform��g�p���ĉ�]��K�p
		// Yaw�p�x����̂܂܎g�p
		FWidgetTransform Transform;
		Transform.Angle = PlayerYaw;
		PlayerDirection->SetRenderTransform(Transform);
	}
}


