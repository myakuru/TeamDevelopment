
#include "FloatingWeaponAttackState.h"

#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>

UFloatingWeaponAttackState::UFloatingWeaponAttackState()
{
}

void UFloatingWeaponAttackState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }
	UE_LOG(LogTemp, Warning, TEXT("AttackState"));
	auto* attack = Owner->GetOwnerAttack();

	if (attack->CanDeactivate())
	{
		Owner->ChangeState(EFloatingWeaponState::Transition,EFloatingWeaponState::Stand);
		return;
	}

	Transform = CalcAttackStateTransform(attack,attack->CurrentAngle);
	
	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponAttackState::CalcAttackStateTransform(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle)
{
	if (!OwnerActor || !OwnerAttack) { return FTransform(); }

	FTransform resultTransform;

	// プレイヤーの座標
	const FVector playerLocation = OwnerActor->GetActorLocation();

	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();

	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = OwnerAttack->CalcAttackDir(playerForwardVector) * RadiusOffset;

	// 浮遊武器の最終位置
	const FVector resultLocation = playerLocation + offsetLocation;

	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw + RotatorOffsetAngle;

	resultTransform.SetLocation(resultLocation);
	resultTransform.SetRotation(RotatorOffset.Quaternion());

	return resultTransform;
}
