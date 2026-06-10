#include "TreasureBox.h"

#include <ProjectNull/Actor/Map/MapActorManager.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/SaveGame/MySaveGame.h>

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
	auto* MapActorMan = GetWorld()->GetGameInstance<USuperGameInstance>()->GetMapActorManager();
	if (TreasureID.IsNone() == false && MapActorMan){
		//DestroyedFromSaveDataがfalseならセーブデータによる自壊はしない
		if (!DestroyedFromSaveData) {
			MapActorMan->RemoveDestroyedActor(TreasureID);
		}
		else if (MapActorMan->IsDestroyedActor(TreasureID))
		{
			Destroy();
			return;
		}
	}

	// ディゾルブ用マテリアルインスタンスの生成
	if (Mesh)
	{
		DynamicMaterial = Mesh->CreateAndSetMaterialInstanceDynamic(0);
		Mesh->SetMaterial(0, DynamicMaterial);
	}
}

void ATreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    if (!bDissolving) return;

    DissolveAmount += DeltaTime * DissolveSpeed;

    DynamicMaterial->SetScalarParameterValue(
        TEXT("DissolveAmount"),
        DissolveAmount
    );

    if (DissolveAmount >= 1.0f)
    {
        Destroy();
    }
}

void ATreasureBox::HitReaction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* pAwn = Cast<APawn>(OtherActor);
	if (!pAwn) return;

	// プレイヤーだけにしたい場合
	if (!pAwn->IsPlayerControlled()) return;

	//状態保存(ステージクリア時にセーブするように変更必須！！)
	auto* MapActorMan = GetWorld()->GetGameInstance<USuperGameInstance>()->GetMapActorManager();
	if (!TreasureID.IsNone() && MapActorMan && DestroyedFromSaveData)
	{
		MapActorMan->RegisterDestroyedActor(TreasureID);
	}

	//アニメーション再生
	if (!Mesh || !OpenAnimation) return;
	Mesh->PlayAnimation(OpenAnimation, false);

	//消滅タイマー
	float AnimationLength = OpenAnimation->GetPlayLength();
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ATreasureBox::ExtinctionStart,
		AnimationLength,
		false
	);

	// 経験値ドロップ
	//if (UItemManagerSubsystem* ItemSubsystem =
	//	GetWorld()->GetSubsystem<UItemManagerSubsystem>())
	//{
	//	const FLinearColor Color = DropItemParams.ExpColor;
	//	const float Size = DropItemParams.ExpSize;

	//	ItemSubsystem->GetExperiencePickupManager().SpawnExperience(
	//		GetActorLocation(),
	//		static_cast<float>(DropItemParams.DropExp),
	//		Color,
	//		Size
	//	);
	//}

	//// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	//if (USuperGameInstance* GameInstance =
	//	GetWorld()->GetGameInstance<USuperGameInstance>())
	//{
	//	GameInstance->GetPlayerRuntimeData()->AddExperience(DropItemParams.DropExp);
	//}
}
