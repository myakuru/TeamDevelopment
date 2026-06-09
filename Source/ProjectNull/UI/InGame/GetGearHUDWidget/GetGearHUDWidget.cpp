#include "GetGearHUDWidget.h"
#include "Components/TextBlock.h"
#include "Components/Image.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Weapon/Manager/WeaponManager.h>
#include <ProjectNull/Weapon/Data/WeaponData.h>

void UGetGearHUDWidget::SetGearData(const FText& inGearName)
{
	UWeaponManager* weaponMan =
		GetWorld()->GetGameInstance<USuperGameInstance>()
		->GetWeaponManager();

	FWeaponData weaponData;
	if (!weaponMan->GetWeaponMaster(FName(*inGearName.ToString()), weaponData))return;

	if (GearName) {
		GearName->SetText(weaponData.DisplayName);
	}

	if (GearImage) {
		GearImage->SetBrushFromTexture(weaponData.Icon);
	}
}
