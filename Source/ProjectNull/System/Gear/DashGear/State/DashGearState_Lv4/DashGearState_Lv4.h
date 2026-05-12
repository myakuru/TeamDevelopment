#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include "DashGearState_Lv4.generated.h"

/** ダッシュギア必殺技の高速移動データ */
USTRUCT(BlueprintType)
struct FDashSpecialMoveData
{
	GENERATED_BODY()
public:
	FDashSpecialMoveData():
		AngleOffset(0.0f),
		Time(0.0f),
		Speed(0.0f)
	{
	}
public:

	UPROPERTY(EditAnywhere)
	float AngleOffset;

	UPROPERTY(EditAnywhere)
	float Time;

	UPROPERTY(EditAnywhere)
	float Speed;
};

class UAnimationAsset;

/** ダッシュギアのレベル4状態クラス */
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv4 final : public UDashGearStateBase
{
	GENERATED_BODY()
public:
	UDashGearState_Lv4();
public:
	void Initialize(class APlayerBase* Player,
		class UPlayerGearComponent* GearComponent,
		class UGearBase* Gear)		override;
	void Update(float DeltaTime)	override;
private:
	UPROPERTY(EditAnywhere)
	FVector StartPlayerForwardVector;

	UPROPERTY(EditAnywhere)
	int32 DirCount;

	/** ダッシュギア必殺技の高速移動データ */
	UPROPERTY(EditAnywhere)
	TArray<FDashSpecialMoveData> DashSpecialMoveDataArray;

	UPROPERTY(EditAnywhere, Category = "Ghost")
	TObjectPtr<UMaterialInterface> GhostMaterial;

	UPROPERTY(EditAnywhere)
	UAnimationAsset* Animation;

	UPROPERTY(EditAnywhere)
	float AnimTime;
};
