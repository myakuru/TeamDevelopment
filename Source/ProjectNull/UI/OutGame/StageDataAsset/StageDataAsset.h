#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StageDataAsset.generated.h"

/* ドロップギア構造体 */
USTRUCT(BlueprintType)
struct FStageDropGearDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	/** ギア情報 */
	UPROPERTY(EditAnywhere)
	FDataTableRowHandle WeaponRow;

	/** 抽選の重み付け */
	UPROPERTY(EditAnywhere)
	int32 Weight = 1;
};

/** ステージのデータ構造体 */
USTRUCT(BlueprintType)
struct FStageDataStruct : public FTableRowBase
{
	GENERATED_BODY()

	/** ステージの名前 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText StageName;

	/** ステージの画像 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UTexture2D> StageImage;

	/** 開くレベルの名前 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName LevelName;

	/** ドロップギアデータ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FStageDropGearDataStruct> DropGearData;
}; 

/** ステージのデータアセットクラス */
UCLASS(BlueprintType)
class PROJECTNULL_API UStageDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	/** ステージのデータ構造体 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "StageData　Setting")
	TArray<FStageDataStruct> StageData;

	//ランダムに抽選する
	FDataTableRowHandle DropRandomGear(int stageIndex)
	{
		// 抽選の範囲を設定
		int32 totalWeight = 0;
		for (const FStageDropGearDataStruct& obtainableWeapon :
			StageData[stageIndex].DropGearData) {
			totalWeight += obtainableWeapon.Weight;
		}
		if (!totalWeight) return FDataTableRowHandle();

		// 抽選の範囲からランダム値取得
		const int32 randomValue = FMath::RandRange(0, totalWeight - 1);

		// 抽選結果反映
		int32 currentWeight = 0;
		for (const FStageDropGearDataStruct& obtainableWeapon :
			StageData[stageIndex].DropGearData) {
			currentWeight += obtainableWeapon.Weight;

			if (randomValue < currentWeight) {
				return obtainableWeapon.WeaponRow;
			}
		}

		return FDataTableRowHandle();
	}
};
