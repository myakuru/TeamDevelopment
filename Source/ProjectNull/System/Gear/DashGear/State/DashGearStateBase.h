#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "DashGearStateBase.generated.h"

/** ナイアガラシステムクラス */
class UNiagaraSystem;

/** アニメーションモンタージュ */
class UAnimMontage;

class UEffectBase;

class UDashGear;

UCLASS()
class PROJECTNULL_API UDashGearStateBase : public UGearStateBase
{
	GENERATED_BODY()
public:	
	UDashGearStateBase();
public:

	virtual void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner) override;
	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;
	virtual void End()									override;

protected:
	
	void InitializeStartDashData(USceneComponent* InGroundAlignmentComp);

	void Dash();

	void PlayDashNiagaraEffect(USceneComponent* InGroundAlignmentComp);
	void DeactivateNiagaraEffect();

	void PlayDashAnimation();
	void BlendOutDashAnimation();

	void SetSphereCollisionEnabled(const ECollisionEnabled::Type InEnabled);

	void SetEnableSpawnAfterimage(bool bInEnableSpawn);

	UPROPERTY(EditDefaultsOnly, Category = "Animation")
	TObjectPtr<UAnimMontage> DashAnimMontage;

	/** ダッシュを表現するエフェクトActor */
	UPROPERTY(EditAnywhere, Instanced, Category = "Effect")
	TObjectPtr<UEffectBase> DashEffect;

private:

	UPROPERTY()
	TObjectPtr<UDashGear> DashGear;

	UPROPERTY()
	FVector DashDir;

	UPROPERTY()
	FQuat StartQuat;

	UPROPERTY(EditAnywhere)
	float DashSpeed;

	UPROPERTY(EditAnywhere)
	float DashEffectDuration;

	UPROPERTY(EditAnywhere)
	float MontageBlendOutTime;
};
