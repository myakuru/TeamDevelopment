
#include "FloatingWeaponAttackState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponAttackState::UFloatingWeaponAttackState()
{
}

void UFloatingWeaponAttackState::Update(AActor* OwnerActor, float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	auto* attack = Owner->GetOwnerAttack();

	// 攻撃が消えたらエフェクト消す
	if (attack->CanDeactivate())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}

	// プレイヤーの座標
	const FVector playerLocation = OwnerActor->GetActorLocation();
	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();
	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = attack->CalcAttackDir(playerForwardVector) * RadiusOffset;
	// 浮遊武器の最終位置
	const FVector resultLocation = playerLocation + offsetLocation;

	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw + attack->CurrentAngle;

	Transform.SetLocation(resultLocation);
	Transform.SetRotation(RotatorOffset.Quaternion());

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}

