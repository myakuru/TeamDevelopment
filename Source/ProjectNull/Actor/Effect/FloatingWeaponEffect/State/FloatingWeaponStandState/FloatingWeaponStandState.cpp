

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

	// �v���C���[�̍��W
	const FVector playerLocation = OwnerActor->GetActorLocation();
	// �v���C���[�������Ă����
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();
	const FVector playerRightVector = OwnerActor->GetActorRightVector();
	// �U����������̃I�t�Z�b�g�ʒu
	const FVector offsetLocation = playerRightVector * OffsetDist;
	// ���V����̍ŏI�ʒu
	const FVector resultLocation = playerLocation + offsetLocation;

	Transform.SetLocation(resultLocation);
	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw;

	Transform.SetRotation(RotatorOffset.Quaternion());

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}

