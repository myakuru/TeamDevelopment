
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponStandState.generated.h"


UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponStandState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
	
public:
	UFloatingWeaponStandState();
public:
	void Initialize() override;
	void Start()					override;
	void Update(float DeltaTime)	override;

	FTransform GetStartTransformOffset();
	
private:
	UPROPERTY(EditAnywhere)
	FTransform StartTransformOffset;

	// 位相（時間）
	float Phase;

	// 開始位相（時間）
	UPROPERTY(EditAnywhere)
	float StartPhase;

	// 周波数（速さ）
	UPROPERTY(EditAnywhere)
	float Frequency;

	// 振幅（幅）
	UPROPERTY(EditAnywhere)
	float Amplitude;
};
