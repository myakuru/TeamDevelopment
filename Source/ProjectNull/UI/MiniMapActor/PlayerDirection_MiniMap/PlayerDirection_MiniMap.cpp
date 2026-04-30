// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerDirection_MiniMap.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

void UPlayerDirection_MiniMap::NativeConstruct()
{
	Super::NativeConstruct();
	
	// NativeTick()を有効化
	SetIsFocusable(false);
}

void UPlayerDirection_MiniMap::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	// プレイヤーのヨー角を取得
	if (APlayerBase* Player = Cast<APlayerBase>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)))
	{
		const float PlayerYaw = Player->GetActorRotation().Yaw;

		// プレイヤーの向きを更新(UI)
		UpdatePlayerDirection(PlayerYaw);
	}
}

void UPlayerDirection_MiniMap::UpdatePlayerDirection(float PlayerYaw)
{
	if (PlayerDirection)
	{
		// プレイヤーのヨー角を回転に変換してUIに反映
		FWidgetTransform Transform;
		Transform.Angle = PlayerYaw;
		PlayerDirection->SetRenderTransform(Transform);
	}
}


