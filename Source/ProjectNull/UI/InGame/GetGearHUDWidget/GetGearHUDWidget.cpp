#include "GetGearHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/Weapon/Data/WeaponData.h>

void UGetGearHUDWidget::SetGearData(const FName& inGearName)
{
	UWeaponManager* weaponMan =
		GetWorld()->GetGameInstance<USuperGameInstance>()
		->GetWeaponManager();

	FWeaponData weaponData;
	if (!weaponMan->GetWeaponMaster(inGearName, weaponData))return;

	if (GearName) {
		GearName->SetText(weaponData.DisplayName);
	}

	if (GearImage) {
		GearImage->SetBrushFromTexture(weaponData.Icon);
	}
}

void UGetGearHUDWidget::OpenUI()
{
	//ザ・ワールド
	UGameplayStatics::SetGamePaused(
		GetWorld(),
		true
	);

	//マウスカーソル表示
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();

	if (!PC)return;

	PC->bShowMouseCursor = true;

	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(
		TakeWidget()
	);

	PC->SetInputMode(InputMode);

	//フェードインアニメーション再生
	if (FadeInAnim)
	{
		PlayAnimation(FadeInAnim);
	}
}

void UGetGearHUDWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//閉じるボタンに消去イベント登録
	if (CloseUIButton) {
		CloseUIButton->OnClicked.AddUniqueDynamic(this, &UGetGearHUDWidget::RemoveSelf);
	}
}

void UGetGearHUDWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UGetGearHUDWidget::RemoveSelf()
{
	//フェードアウトアニメーション再生
	if (FadeOutAnim)
	{
		PlayAnimation(FadeOutAnim);

		FWidgetAnimationDynamicEvent AnimationFinishedEvent;
		AnimationFinishedEvent.BindDynamic(this, &UGetGearHUDWidget::OnFadeOutAnimFinished);

		BindToAnimationFinished(FadeOutAnim, AnimationFinishedEvent);

		return;
	}

	OnFadeOutAnimFinished();
}

void UGetGearHUDWidget::OnFadeOutAnimFinished()
{
	APlayerController* PC =
		GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;

		FInputModeGameOnly InputMode;
		PC->SetInputMode(InputMode);
	}

	UGameplayStatics::SetGamePaused(
		GetWorld(),
		false
	);

	RemoveFromParent();
}
