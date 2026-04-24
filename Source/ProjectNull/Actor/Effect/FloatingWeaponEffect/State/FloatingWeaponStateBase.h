
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "../../FloatingWeaponEffect/FloatingWeaponEffect.h"
#include "FloatingWeaponStateBase.generated.h"

// 浮遊武器クラス
class UFloatingWeaponEffect;

/// <summary>
// 浮遊武器の状態基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API UFloatingWeaponStateBase : public UObject
{
	GENERATED_BODY()

public:

	UFloatingWeaponStateBase();

public:
	virtual void Start() { return; }
	virtual void Start(EFloatingWeaponState SetNextState) { return; }

	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(float DeltaTime);

	// セッター
	inline void SetOwner(UFloatingWeaponEffect* SetOwner) { Owner = SetOwner; }
	inline void SetOwnerActor(AActor* SetOwnerActor) { OwnerActor = SetOwnerActor; }

protected:

	inline bool IsFinishedTransitionState() const { return TransitionTime <= 0.0f; }

	float GetTransitionStateTime() const;

	void UpdateTransitionTime(float DeltaTime);

	float TransitionTime;

	UPROPERTY()
	UFloatingWeaponEffect* Owner;

	UPROPERTY()
	AActor* OwnerActor;

	UPROPERTY(EditAnywhere)
	FVector LocationOffset;

	// エフェクトの半径オフセット
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	// エフェクトの回転オフセット
	UPROPERTY(EditAnywhere)
	FRotator RotatorOffset;
	
};
