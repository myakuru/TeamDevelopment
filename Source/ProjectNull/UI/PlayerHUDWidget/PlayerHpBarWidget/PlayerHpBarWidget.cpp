// Fill out your copyright notice in the Description page of Project Settings.
#include "PlayerHpBarWidget.h"

#include "Components/Image.h"

void UPlayerHpBarWidget::SetHp(int32 CurrentHp, int32 MaxHp)
{
	// �v���C���[��Image����ɃX�P�[�����O����HP��\�������
	if (ExpImage)
	{
		float HpRatio = static_cast<float>(CurrentHp) / static_cast<float>(MaxHp);

		// �摜�̍��[���ɃX�P�[�����O���邽�߁A�s�{�b�g��ݒ�
		ExpImage->SetRenderTransformPivot(FVector2D(0.0f, 0.5f));

		// HP�̊����ɉ����ĉ摜��X�P�[�����O
		ExpImage->SetRenderScale(FVector2D(HpRatio, 1.0f));
	}
}
