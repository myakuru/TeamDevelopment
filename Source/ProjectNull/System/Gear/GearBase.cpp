#include "GearBase.h"

#include "../../Actor/Character/Player/PlayerBase.h"
#include "../../Component/PlayerGearComponent/PlayerGearComponent.h"

void UGearBase::Initialize(APlayerBase* Player, UPlayerGearComponent* GearComponent)
{
	OwnerPlayer			= Player;
	OwnerGearComponent	= GearComponent;

}

void UGearBase::Update(float DeltaTime)
{
	if (!OwnerGearComponent) { return; }

	//　現在のギアレベルを取得
	const int32 currentGearLevel = OwnerGearComponent->GetCurrentGearLevel();

	//　現在のギアレベルに応じた効果の適用
	switch (currentGearLevel) 
	{
	case 1:ApplyLv1();break;
	case 2:ApplyLv2();break;
	case 3:ApplyLv3();break;
	case 4:ApplyLv4();break;
	default:return;
	}
}
