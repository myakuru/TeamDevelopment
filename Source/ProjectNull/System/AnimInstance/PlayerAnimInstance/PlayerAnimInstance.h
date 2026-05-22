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
	
	/** Getter */
	FPoseSnapshot& GetPlayerPoseSnapshot();

	/** 戦闘構え状態か */
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatStance;

private:

	/**
	 * @brief 現在のポーズをプレイヤーのポーズへ保存
	 */
	void SavePoseSnapshot();

	/**
	 * @brief プレイヤーのポーズ（FPoseSnapshot）を格納する
	 */
	UPROPERTY()
	FPoseSnapshot PlayerPoseSnapshot;
};
