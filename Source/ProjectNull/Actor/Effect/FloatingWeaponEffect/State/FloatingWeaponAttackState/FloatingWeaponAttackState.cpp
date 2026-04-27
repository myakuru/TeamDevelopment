
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

	FCalcResultOffset resultOffset = CalcAttackStateTransformOffset(attack,attack->CurrentAngle);

	// 回転オフセット考慮して計算
	Owner->SetRotatorYawOffset(resultOffset.YawOffset);

	LocationOffset = resultOffset.Transform.GetLocation();

	UFloatingWeaponStateBase::Update(DeltaTime);
}

FCalcResultOffset UFloatingWeaponAttackState::CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle)
{
	FCalcResultOffset resultOffset;

	if (!Owner || !OwnerActor || !OwnerAttack) { return resultOffset; }

	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();

	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = OwnerAttack->CalcAttackDir(FVector::ForwardVector, RotatorOffsetAngle) * RadiusOffset;

	resultOffset.Transform.SetLocation(offsetLocation);
	resultOffset.Transform.SetRotation(Rotation.Quaternion());
	resultOffset.YawOffset = RotatorOffsetAngle;

	return resultOffset;
}
