#include "EnemyBossAnimInstance.h"
#include "GameFramework/Pawn.h"

// アニメーションインスタンスの初期化
void UEnemyBossAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	// 自分が紐づくPawnを取得して保持。AIControllerではなくPawnを見るのがポイント
	OwnerPawn = TryGetPawnOwner();
}

// アニメーションインスタンスの更新
void UEnemyBossAnimInstance::NativeThreadSafeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	if (!IsValid(OwnerPawn)) { return; }

	// 速度はPawn（CharacterMovement）が持っている。誰が操作していても同じ
	const FVector Velocity = OwnerPawn->GetVelocity();
	MoveSpeed = Velocity.Size2D();      // 上下を無視した地上の速さ


}
