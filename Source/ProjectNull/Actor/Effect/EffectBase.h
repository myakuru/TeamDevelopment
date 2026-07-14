
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"

#include "EffectBase.generated.h"

/** Niagaraエフェクトクラス */
class UNiagaraSystem;

/** Niagaraコンポーネントクラス */
class UNiagaraComponent;


/** エフェクト中間基底クラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UEffectBase : public UObject
{
	GENERATED_BODY()
public:
	UEffectBase();

	void Start(USceneComponent* RootComponent);
	void StartOnce(USceneComponent* RootComponent);

	void DeactivateEffect();
	void DeactivateImmediateEffect();

	void SetAbsolute(
		bool bNewAbsoluteLocation,
		bool bNewAbsoluteRotation,
		bool bNewAbsoluteScale);

	inline UNiagaraComponent* GetEffectComponent() const { return EffectComponent; }

protected:

	/** 持ち主のクラス */
	UPROPERTY()
	TObjectPtr<AActor>				OwnerActor;

	/** Niagaraシステム */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem>		EffectSystem;

	/** Niagaraコンポーネント */
	UPROPERTY()
	TObjectPtr<UNiagaraComponent>	EffectComponent;

	/** エフェクトのRelativeTransform */
	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;
};
