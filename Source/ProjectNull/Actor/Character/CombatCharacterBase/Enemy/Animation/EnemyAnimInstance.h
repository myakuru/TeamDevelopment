#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../EnemyDataStruct.h"
#include "EnemyAnimInstance.generated.h"

class AEnemyBase;

UCLASS()
class UEnemyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:

	/** 初期化（ゲームスレッド）。Pawnをキャッシュしておく*/
	virtual void NativeInitializeAnimation()override;

	/** 毎フレームの値更新（ワーカースレッドで並列更新）*/
	virtual void NativeUpdateAnimation(float DeltaTime)override;

	// AnimBPから参照する変数
	UPROPERTY(BlueprintReadOnly, Category = "State")
	EEnemyState CurrentState = EEnemyState::Idle;

protected:

	/** 持ち主のクラスを取得 */
	const TObjectPtr<AEnemyBase>& GetOwnerEnemy()const { return OwnerEnemy; }

private:

	/**	持ち主クラスのアドレス */
	UPROPERTY()
	TObjectPtr<AEnemyBase> OwnerEnemy = nullptr;
};