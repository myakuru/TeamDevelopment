#pragma once

#include "CoreMinimal.h"
#include "../MapActorBase.h"
#include "TreasureBox.generated.h"

class UGetGearHUDWidget;

/** プレイヤーが近づくと破壊される宝箱 */

 //宝箱のドロップアイテムパラメータ
USTRUCT(BlueprintType)
struct FDropItemParams
{
	GENERATED_BODY()
public:

	/** EXP */
	UPROPERTY(EditAnywhere)
	FLinearColor ExpColor = FLinearColor::Yellow;

	UPROPERTY(EditAnywhere)
	float ExpSize = 1.0f;

	UPROPERTY(EditAnywhere)
	float DropExp = 100.0f;
};

UCLASS()
class PROJECTNULL_API ATreasureBox : public AMapActorBase
{
	GENERATED_BODY()

public:
	ATreasureBox();

	void SetDropItemParams(const FDropItemParams& Params) { DropItemParams = Params; }
public:

	void BeginPlay() override;

	void Tick(float DeltaTime) override;

	void ExtinctionStart();

	void HitReaction(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)override;

	UFUNCTION()
	UGetGearHUDWidget* CreateDropItemWidget(const FText& itemName);

	/** ID */
	//UPROPERTY(EditInstanceOnly, Category = "Params|ID")
	//FName TreasureID = NAME_None;

	UPROPERTY(EditInstanceOnly, Category = "Params|ID")
	bool DestroyedFromSaveData = true;

	/**ディゾルブ用マテリアルインスタンス */
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	/**ディゾルブ */
	UPROPERTY()
	bool bDissolving = false;

	UPROPERTY()
	float DissolveAmount = 0.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Params|Dissolve")
	float DissolveSpeed = 1.0f;

	/** 開くアニメーション */
	UPROPERTY(EditDefaultsOnly, Category = "Params|Animation")
	float OpenAnimationPlayRate = 1.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Params|Animation")
	UAnimationAsset* OpenAnimation;

	/** ドロップギア */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params|DropItem")
	FText DropGearName = FText::FromString("");

	/** ドロップアイテムのパラメータ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Params|DropItem")
	FDropItemParams DropItemParams;

	/** ドロップアイテムのHUDウィジェットクラス */
	UPROPERTY(EditDefaultsOnly, Category = "Params|DropItem")
	TSubclassOf<UGetGearHUDWidget> DropItemWidgetClass;

	//タイマー
	FTimerHandle DestroyTimerHandle;
};
