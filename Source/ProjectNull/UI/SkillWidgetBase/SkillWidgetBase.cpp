// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidgetBase.h"
#include "Components/Image.h"

void USkillWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();
	// �N�[���_�E���p�̃}�e���A����쐬
	if (SkillImage)
	{
		CooldownMaterial = SkillImage->GetDynamicMaterial();
	}

}

void USkillWidgetBase::UpdateRotationImage(float angle)
{
	if(CooldownMaterial)
	{
		// �}�e���A���̃p�����[�^��X�V���ĉ�]�𔽉f������
		CooldownMaterial->SetScalarParameterValue(TEXT("RotationAngle"), angle);
	}
}