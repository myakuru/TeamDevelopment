/**
* アイテム関連を管理するマネージャー
* 各種類のアイテムを管理するマネージャー達を管理する
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "ItemManagerSubsystem.generated.h"

/** アイテムの中間クラス*/
class AItemBase;

/** 拾い集めるアイテムの管理クラス*/
class FPickupItemManager;

struct FPickupItemManagerDeleter
{
	void operator()(FPickupItemManager* Ptr) const;
};

/** アイテム管理クラス*/
UCLASS()
class PROJECTNULL_API UItemManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	/** サブシステムの生成時初期化*/
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/** すべてのアイテムマネージャーの更新*/
	void UpdateItemManagers(float DeltaTime);

	/** 拾い集めるアイテムの登録、削除*/
	void RegisterPickupItem(AItemBase* Item);
	void RemovePickupItem(AItemBase* Item);

	/** 総アイテム数取得*/
	int GetItemNum() const;

private:

	TUniquePtr<FPickupItemManager, FPickupItemManagerDeleter> PickupItemManager;

};
