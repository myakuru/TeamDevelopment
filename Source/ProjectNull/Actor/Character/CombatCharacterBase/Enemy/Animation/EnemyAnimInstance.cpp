#include "EnemyAnimInstance.h"
#include "../EnemyBase.h"

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!OwnerEnemy)
	{
		// 初回だけ取得
		OwnerEnemy = Cast<AEnemyBase>(GetOwningActor());
	}

	if (!OwnerEnemy) { return; }

	// 毎フレーム enum を同期するだけ
	CurrentState = OwnerEnemy->GetEnemyState();
}