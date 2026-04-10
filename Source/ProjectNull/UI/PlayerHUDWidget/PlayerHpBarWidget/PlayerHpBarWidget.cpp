// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerHpBarWidget.h"

#include "Components/Image.h"

void UPlayerHpBarWidget::SetHp(int32 CurrentHp, int32 MaxHp)
{
	// プレイヤーのImageを左にスケーリングしてHPを表現する例
	if (ExpImage)
	{
		float HpRatio = static_cast<float>(CurrentHp) / static_cast<float>(MaxHp);

		// 画像の左端を基準にスケーリングするため、ピボットを設定
		ExpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		// HPの割合に応じて画像をスケーリング
		ExpImage->SetRenderScale(FVector2D(HpRatio, 1.0f));
	}
}
