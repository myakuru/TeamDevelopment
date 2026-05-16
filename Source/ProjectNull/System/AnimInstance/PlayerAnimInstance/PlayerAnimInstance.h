#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/** プレイヤーのアニメーションインスタンスクラス */
UCLASS()
class PROJECTNULL_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UPlayerAnimInstance();
public:
	/** 戦闘構え状態か */
	UPROPERTY(BlueprintReadOnly)
	bool bIsCombatStance;

	/** 現在のアニメーションアセット */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TObjectPtr<UAnimationAsset> CurrentAnimationAsset;

	/** 現在のアニメーション閾値 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentAnimationTime;

};
