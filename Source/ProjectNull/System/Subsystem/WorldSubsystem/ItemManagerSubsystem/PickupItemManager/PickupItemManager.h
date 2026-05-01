/**
* 拾うアイテム群をまとめて管理するマネージャークラス
* UObjectではなく、純粋なC++クラスで作ることで軽量に
*/

#pragma once

#include "CoreMinimal.h"

class AItemBase;

class FPickupItemManager
{
public:

	/** 登録されているPickupItemをまとめて更新*/
	void Update(APawn* Player, float DeltaTime);

	/** PickupItemを管理リストへ登録する*/
	void Register(AItemBase* Item);
	/** PickupItemを管理リストから削除する*/
	void Remove(AItemBase* Item);

	/** 現在管理しているアイテム数を渡す*/
	int32 GetItemNum() const { return PickupItemList.Num(); }

	/** リストを全削除する*/
	void Clear();

private:

	/** PickupItem保持リスト（Destroy済み対策としてWeakPtr使用）*/
	TArray<TWeakObjectPtr<AItemBase>> PickupItemList;

};