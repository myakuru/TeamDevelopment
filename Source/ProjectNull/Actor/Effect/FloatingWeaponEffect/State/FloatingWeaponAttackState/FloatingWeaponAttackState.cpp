
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

	// �U������������G�t�F�N�g����
	if (attack->CanDeactivate())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}
	//3
	
	// �v���C���[�̍��W
	const FVector playerLocation = OwnerActor->GetActorLocation();
	// �v���C���[�������Ă����
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();
	// �U����������̃I�t�Z�b�g�ʒu
	const FVector offsetLocation = attack->CalcAttackDir(playerForwardVector) * RadiusOffset;
	// ���V����̍ŏI�ʒu
	const FVector resultLocation = playerLocation + offsetLocation;

	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw + attack->CurrentAngle;

	Transform.SetLocation(resultLocation);
	Transform.SetRotation(RotatorOffset.Quaternion());

	UFloatingWeaponStateBase::Update(OwnerActor, DeltaTime);
}

