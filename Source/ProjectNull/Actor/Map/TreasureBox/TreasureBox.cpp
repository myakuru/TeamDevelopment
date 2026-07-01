#include "TreasureBox.h"

//#include <ProjectNull/Actor/Map/MapActorManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Stage/Manager/StageManager.h>
#include <ProjectNull/UI/OutGame/StageDataAsset/StageDataAsset.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/SaveGame/MySaveGame.h>
#include <ProjectNull/UI/InGame/GetGearHUDWidget/GetGearHUDWidget.h>
#include <ProjectNull/Weapon/Data/WeaponData.h>

ATreasureBox::ATreasureBox()
{
	PrimaryActorTick.bCanEverTick = true;

	// イベント登録
	if (Trigger) {
		Trigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&ATreasureBox::HitReaction
		);
	}
}

void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();

	//破壊オブジェクトに登録されているなら自壊する
	//auto* MapActorMan = GetWorld()->GetGameInstance<USuperGameInstance>()->GetMapActorManager();
	//if (TreasureID.IsNone() == false && MapActorMan){
	//	//DestroyedFromSaveDataがfalseならセーブデータによる自壊はしない
	//	if (!DestroyedFromSaveData) {
	//		MapActorMan->RemoveDestroyedActor(TreasureID);
	//	}
	//	else if (MapActorMan->IsDestroyedActor(TreasureID))
	//	{
	//		Destroy();
	//		return;
	//	}
	//}

	//メッシュのマテリアルスロット0のマテリアルを動的インスタンス化して保存
	if (Mesh)
	{
		DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
	}
}

void ATreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!bDissolving) return;

    DissolveAmount += DeltaTime * DissolveSpeed;

	// ディゾルブマテリアルのパラメータを更新(マテリアルの変数名とTEXTが違うと動かない)
	if (DynamicMaterial) {
		DynamicMaterial->SetScalarParameterValue(
			TEXT("DissolveAmount"),
			DissolveAmount
		);
	}

    if (DissolveAmount >= 1.0f)
    {
        Destroy();
    }
}

void ATreasureBox::ExtinctionStart()
{
	bDissolving = true;

	//ドロップギアの名前の有無で決める
	FText dropItemName;
	if (!DropGearName.IsEmpty()) {
		dropItemName = DropGearName;
	}
	else {
		//データからランダムにギアを選んでドロップ
		TObjectPtr<UStageManager> stageMan = GetWorld()->GetGameInstance<USuperGameInstance>()
			->GetStageManagerSubsystem();

		int NowStageIndex = stageMan->GetNowStageIndex();

		auto dataTable = stageMan->GetStageDataAsset()->DropRandomGear(NowStageIndex);
		if (dataTable.IsNull()) return;
		// データテーブルの該当の行を取得 
		FWeaponData* data =
			dataTable.GetRow<FWeaponData>(TEXT("TreasureBox:ExtinctionStart FDataTableRowHandle!!"));
		if (!data) return;

		// FName から FText への変換が必要
		dropItemName = FText::FromName(data->WeaponID);
	}

	//取得ギア追加
	GetWorld()->GetGameInstance<USuperGameInstance>()
		->GetStageManagerSubsystem()->AddAcquiredWeapon(dropItemName);

	CreateDropItemWidget(dropItemName);
}

void ATreasureBox::HitReaction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* pAwn = Cast<APawn>(OtherActor);
	if (!pAwn) return;

	// プレイヤーだけにしたい場合
	if (!pAwn->IsPlayerControlled()) return;

	//状態保存(ステージクリア時にセーブするように変更必須！！)
	//auto* MapActorMan = GetWorld()->GetGameInstance<USuperGameInstance>()->GetMapActorManager();
	//if (!TreasureID.IsNone() && MapActorMan && DestroyedFromSaveData)
	//{
	//	MapActorMan->RegisterDestroyedActor(TreasureID);
	//}

	//アニメーション再生
	if (!Mesh || !OpenAnimation) return;
	Mesh->PlayAnimation(OpenAnimation, false);
	Mesh->SetPlayRate(OpenAnimationPlayRate);

	//消滅タイマー
	float AnimationLength = OpenAnimation->GetPlayLength();
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ATreasureBox::ExtinctionStart,
		AnimationLength / OpenAnimationPlayRate,
		false
	);
}

UGetGearHUDWidget* ATreasureBox::CreateDropItemWidget(const FText& itemName)
{
	UGetGearHUDWidget* widget =
		CreateWidget<UGetGearHUDWidget>(
			GetWorld(),
			DropItemWidgetClass);

	widget->SetGearData(itemName);

	widget->OpenUI();

	widget->AddToViewport(InGameDefinition::GetGearUIZOrder);

	return widget;
}