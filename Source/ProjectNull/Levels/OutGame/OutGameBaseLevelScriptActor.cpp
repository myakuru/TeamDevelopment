#include "OutGameBaseLevelScriptActor.h"

#include "Blueprint/UserWidget.h"

void AOutGameBaseLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	CreateOutGameWidget();
}

void AOutGameBaseLevelScriptActor::CreateOutGameWidget()
{
	if (!CreateOutGameWidgetClass)return;

	UUserWidget* OutGameWidget = CreateWidget<UUserWidget>(GetWorld(), CreateOutGameWidgetClass);
	if (!OutGameWidget)return;

	OutGameWidget->AddToViewport();
}
