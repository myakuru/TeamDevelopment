// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillWidgetBase.h"
#include "Components/Image.h"

void USkillWidgetBase::NativeConstruct()
{
	Super::NativeConstruct();

	if (SkillImage)
	{
		CooldownMaterial = SkillImage->GetDynamicMaterial();
	}

}

void USkillWidgetBase::UpdateRotationImage(float angle)
{
	if(CooldownMaterial)
	{
		CooldownMaterial->SetScalarParameterValue(TEXT("RotationAngle"), angle);
	}
}