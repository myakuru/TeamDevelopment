#include "PickupItemManager.h"
#include <ProjectNull/Actor/Item/ItemBase.h>

void FPickupItemManager::Update(APawn* Player, float DeltaTime)
{
	UE_LOG(LogTemp, Warning, TEXT("Pickup Count: %d"), PickupItemList.Num());

	/** 後ろから回すことで削除時のIndexずれを防ぐ*/
	for (int32 i = PickupItemList.Num() - 1; i >= 0; --i)
	{
		/** WeakPtrから実体取得*/
		AItemBase* Item = PickupItemList[i].Get();

		/** Destroy済みなど無効なら配列から削除*/
		if (!IsValid(Item))
		{
			/** */
			PickupItemList.RemoveAtSwap(i);
			continue;
		}

		UE_LOG(LogTemp, Warning, TEXT("PickupItem Update"));
		/** 各アイテムの更新処理*/
		Item->OnUpdate(Player, DeltaTime);
	}
}

void FPickupItemManager::Register(AItemBase* Item)
{
	/** 無効なら登録しない*/
	if (!Item) { return; }

	PickupItemList.Add(Item);
}

void FPickupItemManager::Remove(AItemBase* Item)
{
	if (!Item) { return; }

	UE_LOG(LogTemp, Warning, TEXT("PickuPIem"));

	/** 指定アイテムを管理対象から外す*/
	PickupItemList.Remove(Item);
}

void FPickupItemManager::Clear()
{
	/** リスト全消去*/
	PickupItemList.Empty();
}