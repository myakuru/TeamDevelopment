
#include "FloatingWeaponEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStateBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponAttackState/FloatingWeaponAttackState.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/State/FloatingWeaponStandState/FloatingWeaponStandState.h>


UFloatingWeaponEffect::UFloatingWeaponEffect():
	OwnerAttack(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	Transform(FTransform()),
	RadiusOffset(200.0f),
	Rotation(FRotator())
{
	
}

void UFloatingWeaponEffect::Initialize()
{
	for (auto& [type, state] : States)
	{
		if (!state) { continue; }
		state->SetOwner(this);
		state->SetOwnerActor(OwnerActor);
	}
	ChangeState(EFloatingWeaponState::Transition);
}

void UFloatingWeaponEffect::Start(USceneComponent* RootComponent)
{
	if (!CanSpawn()) { return; }

	// �G�t�F�N�g�̍Đ��J�n
	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void UFloatingWeaponEffect::Update(float DeltaTime)
{
	if (!OwnerActor || !OwnerAttack || !CurrentState) { return; }

	CurrentState->Update(DeltaTime);

	UpdateTransform();
}

void UFloatingWeaponEffect::ChangeState(EFloatingWeaponState NextState)
{
	if (!States.Contains(NextState) || !States[NextState]) { return; }
	CurrentState = States[NextState];

	if (CurrentState)
	{
		CurrentState->Start();
	}
}

void UFloatingWeaponEffect::ChangeState(EFloatingWeaponState NextState, EFloatingWeaponState TheStateAfterTheNext)
{
	if (!States.Contains(NextState) || !States[NextState]) { return; }
	CurrentState = States[NextState];

	if (CurrentState)
	{
		CurrentState->Start(TheStateAfterTheNext);
	}
}

bool UFloatingWeaponEffect::IsAttackStateStep() const
{
	if (!OwnerAttack) { return false; }
	return OwnerAttack->IsAttackStateStep();
}

FTransform UFloatingWeaponEffect::GetAttackStartTransformOffset()
{
	if (!OwnerActor || !OwnerAttack || !States.Contains(EFloatingWeaponState::Attack)
		|| !States[EFloatingWeaponState::Attack]) { return FTransform(); }
	auto* attakState = Cast<UFloatingWeaponAttackState>(States[EFloatingWeaponState::Attack]);
	FTransform resultTransform;
	if (!attakState) { return resultTransform; }

	resultTransform = attakState->CalcAttackStateTransformOffset(OwnerAttack, OwnerAttack->StartAngle).Transform;
	return resultTransform;
}

FTransform UFloatingWeaponEffect::GetStandStartTransformOffset()
{
	if(!States.Contains(EFloatingWeaponState::Stand)
		|| !States[EFloatingWeaponState::Stand]) { return FTransform(); }
	const auto* standState = Cast<UFloatingWeaponStandState>(States[EFloatingWeaponState::Stand]);
	if (!standState) { return FTransform(); }
	return standState->GetStartTransformOffset();
}

void UFloatingWeaponEffect::UpdateTransform()
{
	CalcTransformOffset();

	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::CalcTransformOffset()
{
	if (!OwnerActor) { return; }

	// �v���C���[�̍��W
	const FVector playerLocation	= OwnerActor->GetActorLocation();
	// �v���C���[�̉�]
	FRotator playerRotation			= OwnerActor->GetActorRotation();

	// ���[���h�I�t�Z�b�g���W
	const FVector worldOffsetLocation	= playerRotation.RotateVector(LocationOffset);
	const FVector resultLocation		= playerLocation + worldOffsetLocation;

	// ����:���W�Ɠ����悤�ɉ�]�I�t�Z�b�g�v�Z��v���C���[�̉�]��l��
	Rotation.Yaw = playerRotation.Yaw + RotatorYawOffset;

	Transform.SetRotation(Rotation.Quaternion());
	Transform.SetLocation(resultLocation);	
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
