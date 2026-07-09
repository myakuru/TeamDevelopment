
#include "FloatingWeaponAttackState.h"

#include <ProjectNull/System/Combat/Attack/FloatingWeaponAttack/FloatingWeaponAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/Actor/HitActor/AutoAttackHitActor/AutoAttackHitActor.h>

UFloatingWeaponAttackState::UFloatingWeaponAttackState()
{
}

void UFloatingWeaponAttackState::Start()
{
	Super::Start();

	if (!Owner) { return; }
	const auto OwnerAttack = Owner->GetOwnerAttack();
	if (!OwnerAttack) { return; }
	const auto AutoAttackHitActor = OwnerAttack->GetAutoAttackHitActor();
	if (!AutoAttackHitActor) { return; }

	AutoAttackHitActor->SetHitEnabled(true);
	
}

void UFloatingWeaponAttackState::Update(float DeltaTime)
{
	if (!Owner) { return; }
	const auto OwnerAttack = Owner->GetOwnerAttack();
	if (!OwnerAttack) { return; }
	
	//UE_LOG(LogTemp, Warning, TEXT("AttackState"));
	
	if (OwnerAttack->CanDeactivate())
	{
		Owner->ChangeState(
			EFloatingWeaponState::Transition,
			EFloatingWeaponState::Stand);
		
		const auto AutoAttackHitActor = OwnerAttack->GetAutoAttackHitActor();
		if (!AutoAttackHitActor) { return; }

		AutoAttackHitActor->SetHitEnabled(false);
		
		return;
	}

	const float CurrentAngle = OwnerAttack->GetCurrentAngle();

	RelativeTransform = CalcAttackStateTransformOffset(OwnerAttack, CurrentAngle);
	
	UFloatingWeaponStateBase::Update(DeltaTime);
}

FTransform UFloatingWeaponAttackState::CalcAttackStateTransformOffset(UFloatingWeaponAttack* OwnerAttack, float RotatorOffsetAngle)
{
	FTransform ResultOffset;

	if (!Owner || !OwnerActor || !OwnerAttack) { return ResultOffset; }

	// 攻撃方向からのオフセット位置
	const FVector Location = OwnerAttack->CalcAttackDir(FVector::ForwardVector, RotatorOffsetAngle) * RadiusOffset;

	RelativeRotation.Yaw = RotatorOffsetAngle;

	ResultOffset.SetLocation(Location);
	ResultOffset.SetRotation(RelativeRotation.Quaternion());

	return ResultOffset;
}
