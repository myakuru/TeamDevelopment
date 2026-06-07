
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EffectBase.generated.h"

/** Niagaraエフェクトクラス */
class UNiagaraSystem;

/** Niagaraコンポーネントクラス */
class UNiagaraComponent;


/** エフェクト中間基底クラス */
UCLASS()
class PROJECTNULL_API UEffectBase : public UObject
{
	GENERATED_BODY()

public:

	UEffectBase();

public:

	void Start(USceneComponent* RootComponent);

protected:

	/** 持ち主のクラス */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	/** Niagaraシステム */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> EffectSystem;

	/** Niagaraコンポーネント */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent> EffectComponent;

	/** エフェクトのRelativeTransform */
	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;
};
