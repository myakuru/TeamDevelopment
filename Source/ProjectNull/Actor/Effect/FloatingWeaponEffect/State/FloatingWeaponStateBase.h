
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
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



	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void Update(AActor* OwnerActor, float DeltaTime);

	// セッター
	inline void SetOnwer(UFloatingWeaponEffect* SetOwner) { Owner = SetOwner; }

protected:

	UPROPERTY()
	UFloatingWeaponEffect* Owner;

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	// エフェクトの半径オフセット
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	// エフェクトの回転オフセット
	UPROPERTY(EditAnywhere)
	FRotator RotatorOffset;

};
