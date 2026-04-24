// Fill out your copyright notice in the Description page of Project Settings.


#include "GearChangeWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void UGearChangeWidget::NativeConstruct()
{
	// �M�A�`�F���W�p��UI�}�e���A����쐬
	if (GearChangeImage)
	{
		GearChangeMaterial = GearChangeImage->GetDynamicMaterial();
	}

	// NativeTick��L����
	SetIsFocusable(false);
}

void UGearChangeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	static float ChargeAmount = 0.0f;

	ChargeAmount += InDeltaTime * 5.0f; // �`���[�W�ʂ���ԂƂƂ�ɑ���������

	SetGearChangeEnergy(ChargeAmount);

}

void UGearChangeWidget::SetGearChangeEnergy(float ChargeAmount)
{
	if (GearChangeMaterial)
	{
		// �}�e���A���̃p�����[�^��X�V���ă`���[�W�ʂ𔽉f������
		GearChangeMaterial->SetScalarParameterValue(TEXT("ChargeAmount"), ChargeAmount);
	}
	if (GearChangeText)
	{
		// �`���[�W�ʂ�0����1000�͈̔͂ɐ���
		ChargeAmount = std::clamp(ChargeAmount, 0.0f, 1000.0f);

		// �`���[�W�ʂ�e�L�X�g�ɕ\��
		FString ChargeText = FString::Printf(TEXT("%.0f"), ChargeAmount);

		GearChangeText->SetText(FText::FromString(ChargeText));
	}
}
