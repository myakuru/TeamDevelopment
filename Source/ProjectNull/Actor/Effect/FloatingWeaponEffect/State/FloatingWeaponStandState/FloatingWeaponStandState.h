
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponStandState.generated.h"

/// <summary>
// ï¿½ï¿½ï¿½Vï¿½ï¿½ï¿½ï¿½Ì‘Ò‹@ï¿½ï¿½ÔƒNï¿½ï¿½ï¿½X
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

	// ƒQƒbƒ^[
	inline FTransform GetStartTransformOffset() const { return StartTransformOffset; }

private:
	// ‘Ò‹@ó‘Ô‚ÌŠJnTranform
	UPROPERTY(EditAnywhere)
	FTransform StartTransformOffset;

	float Angle;
};
