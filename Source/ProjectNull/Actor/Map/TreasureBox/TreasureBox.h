#pragma once

#include "CoreMinimal.h"
#include "../MapActorBase.h"
#include "TreasureBox.generated.h"

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

	void ExtinctionStart(){ bDissolving = true; }

	void HitReaction(
		UPrimitiveComponent* OverlappedComp,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	)override;

	//ディゾルブ用マテリアルインスタンス
	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial;

	//ディゾルブ
	UPROPERTY()
	bool bDissolving = false;

	UPROPERTY()
	float DissolveAmount = 0.0f;

	UPROPERTY(EditAnywhere)
	float DissolveSpeed = 1.0f;

	/** 開くアニメーション */
	UPROPERTY(EditAnywhere)
	UAnimationAsset* OpenAnimation;

	/** ドロップアイテムのパラメータ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "DropItemParams")
	FDropItemParams DropItemParams;

	//タイマー
	FTimerHandle DestroyTimerHandle;
};
