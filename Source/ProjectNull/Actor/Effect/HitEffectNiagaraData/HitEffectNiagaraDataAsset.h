#pragma once
// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Containers/EnumAsByte.h"
#include "Chaos/ChaosEngineInterface.h"
#include "HitEffectNiagaraDataAsset.generated.h"

// ------------------------------------------------------------------------------------
// 前方宣言
// ------------------------------------------------------------------------------------
/** Niagaraエフェクトクラス */
class UNiagaraSystem;
/** Niagaraコンポーネントクラス */
class UNiagaraComponent;


// ------------------------------------------------------------------------------------
// 当たり判定に対応したヒットエフェクトをマップの形で持ったデータアセット
// ------------------------------------------------------------------------------------
UCLASS()
class PROJECTNULL_API UHitEffectNiagaraDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:

	/** 材質ごとのヒットエフェクト*/
	UPROPERTY(EditAnywhere, Category = "Effect")
	TMap<TEnumAsByte<EPhysicalSurface>, TObjectPtr<UNiagaraSystem>> SurfaceEffects;

	/** 材質が一致しないときに出すデフォルトエフェクト*/
	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> DefaultEffect;

};
