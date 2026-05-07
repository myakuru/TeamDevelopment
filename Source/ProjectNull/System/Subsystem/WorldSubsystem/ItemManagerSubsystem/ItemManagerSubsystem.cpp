#include "ItemManagerSubsystem.h"
#include "PickupItemManager/PickupItemManager.h"
#include "ExperiencePickupManager/ExperiencePickupManager.h"
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

	ExperiencePickupManager = TUniquePtr<FExperiencePickupManager, FExperiencePickupManagerDeleter>(
		new FExperiencePickupManager()
	);

	// Worldを渡す
	ExperiencePickupManager->Initialize(GetWorld());
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

	if (ExperiencePickupManager)
	{
		ExperiencePickupManager->Update(pPlayerPawn, DeltaTime);

		const float GainedExp = ExperiencePickupManager->ConsumeCollectedExp();
		if (GainedExp > 0.0f)
		{
			// TODO: Player の AddExp(GainedExp) を呼ぶ
		}
	}

	/** */
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

void UItemManagerSubsystem::Deinitialize()
{
	if (ExperiencePickupManager) { ExperiencePickupManager->Clear(); }
	ExperiencePickupManager.Reset();
	PickupItemManager.Reset();
	Super::Deinitialize();
}

FExperiencePickupManager& UItemManagerSubsystem::GetExperiencePickupManager()
{
	check(ExperiencePickupManager.IsValid());
	return *ExperiencePickupManager;
}

void FPickupItemManagerDeleter::operator()(FPickupItemManager* Ptr) const
{
	delete Ptr;
}

void FExperiencePickupManagerDeleter::operator()(FExperiencePickupManager* Ptr) const
{
	delete Ptr;
}
