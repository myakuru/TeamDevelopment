#include "ButtonBaseWidget.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Sound/SoundManager.h>

void UButtonBaseWidget::NativeConstruct()
{
	if (!Button)return;

	Button->bIsVariable = true;

	Button->OnHovered.AddDynamic(this, &UButtonBaseWidget::OnHoveredButton);
	Button->OnClicked.AddDynamic(this, &UButtonBaseWidget::OnClickedButton);
}

void UButtonBaseWidget::OnHoveredButton()
{
	if (HoverSound)
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
			GetSoundManager()->Play2D(HoverSound);
	}
	
	DoHoveredButton();
}

void UButtonBaseWidget::DoHoveredButton()
{
	if (!OnHovered.IsBound())return;
	OnHovered.Broadcast();
}

void UButtonBaseWidget::OnClickedButton()
{
	if (ClickSound)
	{
		GetWorld()->GetGameInstance<USuperGameInstance>()->
		GetSoundManager()->Spawn2D(ClickSound,1.0f,1.0f,0.0f,
			nullptr,true,true);
	}
	
	DoClickedButton();
}

void UButtonBaseWidget::DoClickedButton()
{
	if (!OnClicked.IsBound())return;
	OnClicked.Broadcast();
}
