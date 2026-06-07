#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "DashGearStateBase.generated.h"

/** ナイアガラシステムクラス */
class UNiagaraSystem;

/** アニメーションモンタージュ */
class UAnimMontage;

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

	UPROPERTY(EditAnywhere, Category = "Effect")
	TObjectPtr<UNiagaraSystem> DashEffect;

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DashAnimMontage;

private:

	void UpdateDashAttack();

	UPROPERTY(EditAnywhere)
	float DashAttackRangeSquared;

	UPROPERTY(EditAnywhere)
	float DashSpeed;

	UPROPERTY(EditAnywhere)
	float DashEffectDuration;
};
