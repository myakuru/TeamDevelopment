
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "PlayerMaterialCollectionUpdater.generated.h"

/** プレイヤー中間基底クラス */
class APlayerBase;

/** プレイヤークラスからMaterial Parameter Collectionへの更新処理クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UPlayerMaterialCollectionUpdater : public UObject
{
	GENERATED_BODY()
public:
	UPlayerMaterialCollectionUpdater();
public:

	/**
	 * @brief 初期化
	 * @param InOwnerPlayer 持ち主のクラス(プレイヤー)
	 */
	void Initialize(APlayerBase* InOwnerPlayer);

	/**
	 * @brief 更新
	 * @param DeltaTime デルタタイム
	 */
	void Update(float DeltaTime);

private:

	/**
	 * @brief マップ用マテリアルコレクションの更新
	 * @param DeltaTime デルタタイム
	 */
	void UpdateMapMaterialCollection(float DeltaTime);

	/** 持ち主のクラス(プレイヤー) */
	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	/** マップ用マテリアルコレクション */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UMaterialParameterCollection> MapMaterialCollection;

};
