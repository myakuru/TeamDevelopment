
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "GhostActorAnimInstance.generated.h"

/** 残像アクターアニメーションインスタンスクラス
	※残像クラスがアニメーションを持ってる場合使用する */
UCLASS()
class PROJECTNULL_API UGhostActorAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	UGhostActorAnimInstance();
public:
	/** 残像のスナップショット */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FPoseSnapshot GhostPoseSnapshot;
};
