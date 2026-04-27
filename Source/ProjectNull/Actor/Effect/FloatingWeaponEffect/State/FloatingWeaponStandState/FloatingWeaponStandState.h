
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponStandState.generated.h"

/// <summary>
// ���V����̑ҋ@��ԃN���X
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

	// �Q�b�^�[
	inline FTransform GetStartTransformOffset() const { return StartTransformOffset; }

private:
	// �ҋ@��Ԃ̊J�nTranform
	UPROPERTY(EditAnywhere)
	FTransform StartTransformOffset;

	float Angle;
};
