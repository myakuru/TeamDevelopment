#pragma once

// ------------------------------------------------------------------------------------
// Includes
// ------------------------------------------------------------------------------------
#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyBossAnimInstance.generated.h"

UCLASS()
class PROJECTNULL_API UEnemyBossAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	// ------------------------------------------------------------------------------------
	// public method
	// ------------------------------------------------------------------------------------

	/** 初期化（ゲームスレッド）。Pawnをキャッシュしておく*/
	virtual void NativeInitializeAnimation()override;

	/** 毎フレームの値更新（ワーカースレッドで並列更新）*/
	virtual void NativeThreadSafeUpdateAnimation(float DeltaSeconds)override;

	/** 水平方向の速さ。StateMachineの遷移に使う*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Anim")
	float MoveSpeed = 0.0f;

private:

	// ------------------------------------------------------------------------------------
	// private valiables
	// ------------------------------------------------------------------------------------

	// このAnimInstanceが動かしているPawn
	UPROPERTY(Transient)
	TObjectPtr<APawn> OwnerPawn = nullptr;

};
