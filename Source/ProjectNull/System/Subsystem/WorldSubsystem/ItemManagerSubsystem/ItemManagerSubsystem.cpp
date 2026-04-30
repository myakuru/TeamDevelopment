#include "ItemManagerSubsystem.h"
#include "PickupItemManager/PickupItemManager.h"
#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Actor/Item/ItemBase.h>

void UItemManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	/** Pickup管理クラス生成*/
	//PickupItemManager = MakeUnique<FPickupItemManager>();

	PickupItemManager = TUniquePtr<FPickupItemManager, FPickupItemManagerDeleter>(
		new FPickupItemManager()
	);
}

void UItemManagerSubsystem::UpdateItemManagers(float DeltaTime)
{

	// プレイヤーの情報を取得する（0番:1P）
	APawn* pPlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	if (!pPlayerPawn) { return; }

	/** Pickup系更新*/
	if (!PickupItemManager) { return; }
	UE_LOG(LogTemp, Warning, TEXT("ItemManager PickupManager Update"));
	PickupItemManager->Update(pPlayerPawn, DeltaTime);
}

void UItemManagerSubsystem::RegisterPickupItem(AItemBase* Item)
{
	if (!PickupItemManager) { return; }
	PickupItemManager->Register(Item);
}

void UItemManagerSubsystem::RemovePickupItem(AItemBase* Item)
{
	if (!PickupItemManager) { return; }
	PickupItemManager->Remove(Item);
}

int32 UItemManagerSubsystem::GetItemNum()const
{
	if (PickupItemManager)
	{
		return PickupItemManager->GetItemNum();
	}
	else
	{
		return 0;
	}
}

void FPickupItemManagerDeleter::operator()(FPickupItemManager* Ptr) const
{
	delete Ptr;
}