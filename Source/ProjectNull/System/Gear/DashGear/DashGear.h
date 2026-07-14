#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/GearBase.h>

#include "DashGear.generated.h"

class ASphereCollision;
class UPlayerRuntimeData;

/** ダッシュギアクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UDashGear final : public UGearBase
{
	GENERATED_BODY()
public:
	UDashGear();

	void Initialize(
		class APlayerBase* Player,
		class UPlayerGearComponent* GearComponent)	override;
	void Execute(int32 CurrentGearLevel)			override;
	void Update(float DeltaTime)					override;

	void SetSphereCollisionEnabled(const ECollisionEnabled::Type InEnabled);
	void SetSphereTransform(const FTransform& Transform);
	void SetSphereRadius(float Radius);

private:


	UFUNCTION()
	void OnDashGearAttackBeginOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	/** 攻撃スフィア判定用 */
	UPROPERTY()
	TObjectPtr<ASphereCollision>	SphereCollision;

	/** 攻撃スフィア判定用クラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<ASphereCollision>	SphereCollisionClass;

	/** プレイヤーのRuntimeDataクラス */
	UPROPERTY()
	TObjectPtr<UPlayerRuntimeData>	PlayerRuntimeData;
	
	UPROPERTY(EditAnywhere)
	float AttackPowerScale;
};
