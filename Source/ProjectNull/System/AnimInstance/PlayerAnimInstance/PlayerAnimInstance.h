#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/** プレイヤーのアニメーションインスタンスクラス */
UCLASS()
class PROJECTNULL_API UPlayerAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();
public:

	void NativeUpdateAnimation(float DeltaSeconds) override;

	inline FPoseSnapshot& GetPlayerPoseSnapshot() { return PlayerPoseSnapshot; }

	/** 戦闘構え状態か */
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatStance;

private:

	void SavePoseSnapshot();

	UPROPERTY()
	FPoseSnapshot PlayerPoseSnapshot;
};
