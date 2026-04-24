
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

	FTransform TransformOffset = CalcAttackStateTransformOffset(attack,attack->CurrentAngle);

	Owner->SetRotatorYawOffset(RotatorOffset.Yaw);

	LocationOffset = TransformOffset.GetLocation();

	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponAttackState::CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle)
{
	if (!Owner || !OwnerActor || !OwnerAttack) { return FTransform(); }
	FTransform resultTransformOffset;

	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();

	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = OwnerAttack->CalcAttackDir(FVector::ForwardVector, RotatorOffsetAngle) * RadiusOffset;

	// 回転オフセット考慮して計算
	//RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw + RotatorOffsetAngle;
	RotatorOffset.Yaw = RotatorOffsetAngle;

	resultTransformOffset.SetLocation(offsetLocation);
	resultTransformOffset.SetRotation(RotatorOffset.Quaternion());
	
	return resultTransformOffset;
}
