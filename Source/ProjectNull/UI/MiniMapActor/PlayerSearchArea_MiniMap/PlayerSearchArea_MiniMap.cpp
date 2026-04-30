// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSearchArea_MiniMap.h"
#include "Components/Image.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include "Kismet/GameplayStatics.h"

void UPlayerSearchArea_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(false);
}

void UPlayerSearchArea_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if(APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		const float CameraYaw = Player->GetControlRotation().Yaw;

		// プレイヤーの索敵範囲を更新(UI)
		UpdatePlayerSearchArea(CameraYaw);
	}
}

void UPlayerSearchArea_MiniMap::UpdatePlayerSearchArea(float CameraYaw)
{
	if (PlayerSearchArea)
	{
		FWidgetTransform Transform;
		Transform.Angle = CameraYaw;
		PlayerSearchArea->SetRenderTransform(Transform);
	}
}
