#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/State/GearStateBase.h>

#include "DashGearStateBase.generated.h"

namespace SEIndex {
	inline constexpr int32 DashSESoundIndex	= 0;
}

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

	virtual void Initialize(
		class APlayerBase* InPlayer,
		class UPlayerGearComponent* InGearComponent,
		class UGearBase* InOwner) override;
	virtual void Execute(int32 CurrentGearLevel)		override;
	virtual void Update(float DeltaTime)				override;
	virtual void End()									override;

protected:
	
	void ExecuteDash();
	void EndDash();

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

	UPROPERTY()
	TObjectPtr<UDashGear> DashGear;

private:

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

	UPROPERTY(EditAnywhere)
	float DashSphereRadius;

	/** ダッシュ可能な地面の最大傾斜角度（度）*/
	UPROPERTY(EditAnywhere)
	float MaxDashSlopeAngle;

	UPROPERTY(EditAnywhere)
	float TargetCameraLagSpeed;
};
