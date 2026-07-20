
#pragma once

#include "CoreMinimal.h"

#include "UObject/Object.h"
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

#include "FloatingWeaponStateBase.generated.h"

/** 浮遊武器クラス */
class UFloatingWeaponEffect;

/** 浮遊武器の状態基底クラス */
UCLASS()
class PROJECTNULL_API UFloatingWeaponStateBase : public UObject
{
	GENERATED_BODY()
public:

	UFloatingWeaponStateBase();
	
	virtual void Initialize() { return; }
	
	virtual void Start() { return; }
	virtual void Start(EFloatingWeaponState SetNextState) { return; }

	/** 更新処理 */
	virtual void Update(float DeltaTime);

	// セッター
	inline void SetOwner(UFloatingWeaponEffect* SetOwner)	{ Owner = SetOwner; }
	inline void SetOwnerActor(AActor* SetOwnerActor)		{ OwnerActor = SetOwnerActor; }

protected:

	inline bool IsFinishedTransitionState() const { return TransitionTime <= 0.0f; }


	float GetTransitionStateTime() const;

	float GetStandStateTime() const;


	void UpdateTransitionTime(float DeltaTime);

	// 状態の遷移時間管理用
	float TransitionTime;

	// 持ち主の浮遊武器クラス
	UPROPERTY()
	TObjectPtr<UFloatingWeaponEffect> Owner;

	// 持ち主のアクタークラス
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;

	// エフェクトの半径オフセット
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	UPROPERTY(EditAnywhere)
	FRotator RelativeRotation;

	UPROPERTY(EditAnywhere)
	FTransform RelativeTransform;
};
