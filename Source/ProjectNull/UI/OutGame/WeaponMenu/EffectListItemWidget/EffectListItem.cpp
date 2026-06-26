// Fill out your copyright notice in the Description page of Project Settings.


#include "EffectListItem.h"
#include <Components/Button.h>
#include <Components/TextBlock.h>
#include <ProjectNull/Weapon/Data/EffectData.h>
#include <Kismet\KismetSystemLibrary.h>

bool UEffectListItem::Initialize()
{
	Super::Initialize();

	UKismetSystemLibrary::PrintString(
		this,
		TEXT("Initialize")
	);

	EffectName->SetText(FText());
	EffectLevel->SetText(FText());

	if (Button) {
		Button->OnClicked.AddDynamic(this, &UEffectListItem::OnClickedButton);
	}
	return false;
}

void UEffectListItem::SetEffectInstance(FEffectInstance InEffectInstance)
{
	EffectInstance = InEffectInstance;

	UKismetSystemLibrary::PrintString(
		this,
		EffectInstance.Data->Name.ToString()
	);

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
