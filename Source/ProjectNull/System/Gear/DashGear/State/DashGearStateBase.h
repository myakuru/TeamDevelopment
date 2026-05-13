#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "DashGearStateBase.generated.h"

/** ナイアガラシステムクラス */
class UNiagaraSystem;


UCLASS()
class PROJECTNULL_API UDashGearStateBase : public UGearStateBase
{
	GENERATED_BODY()

public:	

	UDashGearStateBase();

public:

	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;

protected:
	

	void Dash();

	void PlayDashEffect();

	// ���i�O���j�����a���U���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DashEffect;

private:

	void UpdateDashAttack();

	UPROPERTY(EditAnywhere)
	float DashAttackRangeSquared;

	UPROPERTY(EditAnywhere)
	float DashSpeed;

	UPROPERTY(EditAnywhere)
	float DashEffectDuration;
};
