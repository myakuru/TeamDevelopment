#include "EnemyAnimInstance.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

void UEnemyAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerEnemy = Cast<AEnemyBase>(TryGetPawnOwner());
	if (!IsValid(OwnerEnemy)) { return; }
}

void UEnemyAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerEnemy)) { return; }

	// 毎フレーム enum を同期するだけ
	CurrentState = OwnerEnemy->GetEnemyState();
}