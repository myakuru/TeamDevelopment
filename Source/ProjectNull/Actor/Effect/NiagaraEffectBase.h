
#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Actor.h"

#include "NiagaraEffectBase.generated.h"

/** Niagaraエフェクトクラス */
class UNiagaraSystem;

/** Niagaraコンポーネントクラス */
class UNiagaraComponent;


UCLASS(Blueprintable)
class PROJECTNULL_API ANiagaraEffectBase : public AActor
{
	GENERATED_BODY()
public:	
	ANiagaraEffectBase();
protected:
	virtual void BeginPlay() override;
public:	
	virtual void Tick(float DeltaTime) override;

	void Start(USceneComponent* InRootComponent);

	void DeactivateEffect();

	void SetAbsolute(
		bool bNewAbsoluteLocation,
		bool bNewAbsoluteRotation,
		bool bNewAbsoluteScale);

	inline UNiagaraComponent* GetEffectComponent() const { return EffectComponent; }

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
