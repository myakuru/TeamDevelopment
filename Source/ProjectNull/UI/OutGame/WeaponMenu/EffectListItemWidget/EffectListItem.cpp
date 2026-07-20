// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectListItem.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <ProjectNull/Weapon/Data/EffectData.h>

void UEffectListItem::NativeConstruct()
{
	if (Button) {
		Button->OnClicked.AddUniqueDynamic(this, &UEffectListItem::OnClickedButton);
	}
}

bool UEffectListItem::Initialize()
{
	Super::Initialize();

	EffectName->SetText(FText());
	EffectLevel->SetText(FText());

	
	return false;
}

void UEffectListItem::SetEffectInstance(FEffectInstance InEffectInstance)
{
	EffectInstance = InEffectInstance;

	EffectName->SetText(InEffectInstance.Data->Name);

	if (EffectInstance.Level) {
		FText level = FText::Format(
			FText::FromString(TEXT("+ {0}")),
			FText::AsNumber(EffectInstance.Level)
		);
		EffectLevel->SetText(level);
	}
	else {
		EffectLevel->SetText(FText());
	}

}

void UEffectListItem::OnClickedButton()
{
	OnEffectListItemClicked.Broadcast(EffectInstance);
}
