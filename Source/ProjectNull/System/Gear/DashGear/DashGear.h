#pragma once

#include "CoreMinimal.h"

#include <ProjectNull/System/Gear/GearBase.h>

#include "DashGear.generated.h"

/** 前方宣言 */
class UCollisionAttack;

/** ダッシュギアクラス */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UDashGear final : public UGearBase
{
	GENERATED_BODY()
public:
	UDashGear();
public:

	void Initialize(
		class APlayerBase* Player,
		class UPlayerGearComponent* GearComponent)	override;
	void Execute(int32 CurrentGearLevel)			override;
	void Update(float DeltaTime)					override;

	void SetSphereTransform(const FTransform& Transform) const;

private:

	/**
	 * @brief 球状攻撃クラス配列(ステートレベルが複数あるため)
	 */
	UPROPERTY(EditAnywhere,Instanced)
	TArray<TObjectPtr<UCollisionAttack>> SphereAttacks;

	/**
	 * @brief 現在発動中の攻撃インデックス
	 */
	UPROPERTY()
	int32 CurrentExecuteAttackIndex = 0;
};
