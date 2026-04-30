
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

	const float currentAngle = attack->CurrentAngle;

	RelativeTransform = CalcAttackStateTransformOffset(attack, currentAngle);
	
	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponAttackState::CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle)
{
	FTransform resultOffset;

	if (!Owner || !OwnerActor || !OwnerAttack) { return resultOffset; }

	// 攻撃方向からのオフセット位置
	const FVector location = OwnerAttack->CalcAttackDir(FVector::ForwardVector, RotatorOffsetAngle) * RadiusOffset;

	RelativeRotation.Yaw = RotatorOffsetAngle;

	resultOffset.SetLocation(location);
	resultOffset.SetRotation(RelativeRotation.Quaternion());

	return resultOffset;
}
