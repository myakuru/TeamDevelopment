#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "DashGearStateBase.generated.h"

/** ナイアガラシステムクラス */
class UNiagaraSystem;

/** アニメーションモンタージュ */
class UAnimMontage;

class UEffectBase;

UCLASS()
class PROJECTNULL_API UDashGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
public:	
	UDashGearStateBase();
public:

	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;
	virtual void End()									override;

protected:
	

	void Dash();

	void PlayDashEffect();

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DashAnimMontage;

	/** ダッシュを表現するエフェクトActor */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect")
	TObjectPtr<UEffectBase> DashEffect;

private:

	void UpdateDashAttack();

	UPROPERTY()
	FVector DashDir;

	UPROPERTY()
	FQuat StartQuat;

	UPROPERTY(EditAnywhere)
	float DashSpeed;

	UPROPERTY(EditAnywhere)
	float DashEffectDuration;
};
