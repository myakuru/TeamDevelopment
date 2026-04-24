
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponStandState.generated.h"

/// <summary>
// 浮遊武器の待機状態クラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponStandState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
	
public:
	UFloatingWeaponStandState();
public:
	void Start()					override;
	void Update(float DeltaTime)	override;

	// ゲッター
	inline FTransform GetStartTransformOffset() const { return StartTransformOffset; }

private:
	// 待機状態の開始Tranform
	UPROPERTY(EditAnywhere)
	FTransform StartTransformOffset;

	float Angle;
};
