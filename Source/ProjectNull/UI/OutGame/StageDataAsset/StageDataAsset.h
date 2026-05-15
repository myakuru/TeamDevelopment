#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "StageDataAsset.generated.h"


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

	/** 解放フラグ */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bUnlocked = false;

	/** ハイスコア */
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int32 HighScore = 0;
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
};
