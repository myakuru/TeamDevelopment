// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSearchArea_MiniMap.h"
#include "Components/Image.h"

// �v���C���[�̃N���X�̃w�b�_�[�t�@�C����C���N���[�h
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include "Kismet/GameplayStatics.h"

void UPlayerSearchArea_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();
	// NativeTick��L����
	SetIsFocusable(false);
}

void UPlayerSearchArea_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		const float CameraYaw = Player->GetControlRotation().Yaw;
		// �v���C���[�̕�����X�V
		UpdatePlayerSearchArea(CameraYaw);
	}
}

void UPlayerSearchArea_MiniMap::UpdatePlayerSearchArea(float CameraYaw)
{
	if (PlayerSearchArea)
	{
		// Image��RenderTransform��g�p���ĉ�]��K�p
		// Yaw�p�x����̂܂܎g�p
		FWidgetTransform Transform;
		Transform.Angle = CameraYaw;
		PlayerSearchArea->SetRenderTransform(Transform);
	}
}
