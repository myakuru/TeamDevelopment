

#include "FloatingWeaponStandState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponStandState::UFloatingWeaponStandState()
{
}

void UFloatingWeaponStandState::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	auto* attack = Owner->GetOwnerAttack();

	if (attack->IsActiveFirstFrame())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}

	// プレイヤーの座標
	const FVector playerLocation = OwnerActor->GetActorLocation();
	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();
	const FVector playerRightVector = OwnerActor->GetActorRightVector();
	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = playerRightVector * OffsetDist;
	// 浮遊武器の最終位置
	const FVector resultLocation = playerLocation + offsetLocation;

	Transform.SetLocation(resultLocation);
	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw;

	Transform.SetRotation(RotatorOffset.Quaternion());

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}

