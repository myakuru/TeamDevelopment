
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
	RotatorOffset(FRotator())
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

	// エフェクトの再生開始
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

FTransform UFloatingWeaponEffect::GetAttackStartTransform()
{
	if (!OwnerActor || !OwnerAttack || !States.Contains(EFloatingWeaponState::Attack)
		|| !States[EFloatingWeaponState::Attack]) { return FTransform(); }
	auto* attakState = Cast<UFloatingWeaponAttackState>(States[EFloatingWeaponState::Attack]);
	if (!attakState) { 
		//UE_LOG(LogTemp, Warning, TEXT("aaaa"));
		return FTransform();
	}
	return attakState->CalcAttackStateTransform(OwnerAttack,OwnerAttack->StartAngle);
}

FTransform UFloatingWeaponEffect::GetStandStartTransform()
{
	/*if (!OwnerActor || !OwnerAttack || !States.Contains(EFloatingWeaponState::Stand)
		|| !States[EFloatingWeaponState::Stand]) {
		return FTransform();
	}
	auto* standState = Cast<UFloatingWeaponStandState>(States[EFloatingWeaponState::Stand]);
	if (!standState) {
		return FTransform();
	}*/

	if (!OwnerActor) {
		return FTransform();
	}

	// プレイヤーの座標
	const FVector playerLocation = OwnerActor->GetActorLocation();
	// プレイヤーが向いてる方向
	const FVector playerForwardVector = OwnerActor->GetActorForwardVector();
	// 攻撃方向からのオフセット位置
	const FVector offsetLocation = playerForwardVector * StandStartTransformOffset.GetLocation();
	// 浮遊武器の最終位置
	const FVector resultLocation = playerLocation + offsetLocation;
	FTransform resultTransform;
	resultTransform.SetLocation(resultLocation);
	resultTransform.SetRotation(StandStartTransformOffset.GetRotation() + OwnerActor->GetActorRotation().Quaternion());

	return StandStartTransformOffset;
}

void UFloatingWeaponEffect::UpdateTransform()
{
	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
