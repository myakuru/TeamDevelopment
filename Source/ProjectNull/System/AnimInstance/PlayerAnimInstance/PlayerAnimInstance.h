#pragma once

#include "CoreMinimal.h"

#include "Animation/AnimInstance.h"

#include "PlayerAnimInstance.generated.h"

class APlayerBase;

/** プレイヤーのアニメーションインスタンスクラス */
UCLASS()
class PROJECTNULL_API UPlayerAnimInstance final : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();
public:

	void NativeInitializeAnimation()				override;
	void NativeUpdateAnimation(float DeltaSeconds)	override;

	/** Getter */
	FPoseSnapshot& GetPlayerPoseSnapshot();

	/** 移動すべきかどうか */
	UPROPERTY(BlueprintReadOnly)
	bool bShouldMove;

	/** 落下中かどうか */
	UPROPERTY(BlueprintReadOnly)
	bool bIsFalling;

	/** 戦闘構え状態か */
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatStance;

	/** 速度 */
	UPROPERTY(BlueprintReadOnly)
	FVector Velocity;

	/** 地面での速度 */
	UPROPERTY(BlueprintReadOnly)
	float GroundSpeed;

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

	UPROPERTY()
	TObjectPtr<APlayerBase> Player;

	/** 移動中と判定する速度(cm/s) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float MoveThresholdSpeed;

	/** 上昇中と判定するための最小上向き速度(cm/s) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true"))
	float AscendingVelocityThreshold;
};
