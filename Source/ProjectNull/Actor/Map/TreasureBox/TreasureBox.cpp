#include "TreasureBox.h"

#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ItemManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/ItemManagerSubsystem/ExperiencePickupManager/ExperiencePickupManager.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>

ATreasureBox::ATreasureBox()
{
	PrimaryActorTick.bCanEverTick = true;

	// イベント登録
	if (Trigger) {
		Trigger->OnComponentBeginOverlap.AddDynamic(
			this,
			&AMapActorBase::HitReaction
		);
	}
}

void ATreasureBox::BeginPlay()
{
	Super::BeginPlay();

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
	//Destroy();

	//アニメーション再生
	if (!Mesh || !OpenAnimation) return;
	Mesh->PlayAnimation(OpenAnimation, false);

	//消滅タイマー
	float AnimationLength = OpenAnimation->GetMaxCurrentTime();
	GetWorldTimerManager().SetTimer(
		DestroyTimerHandle,
		this,
		&ATreasureBox::ExtinctionStart,
		AnimationLength,
		false
	);

	// 経験値ドロップ
	if (UItemManagerSubsystem* ItemSubsystem =
		GetWorld()->GetSubsystem<UItemManagerSubsystem>())
	{
		const FLinearColor Color = DropItemParams.ExpColor;
		const float Size = DropItemParams.ExpSize;

		ItemSubsystem->GetExperiencePickupManager().SpawnExperience(
			GetActorLocation(),
			static_cast<float>(DropItemParams.DropExp),
			Color,
			Size
		);
	}

	// ゲームインスタンス経由で、経験値とギアエネルギーをセット
	if (USuperGameInstance* GameInstance =
		GetWorld()->GetGameInstance<USuperGameInstance>())
	{
		GameInstance->GetPlayerRuntimeData()->AddExperience(DropItemParams.DropExp);
	}
}
