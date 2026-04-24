
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

	CalcTransformOffset();
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
	if (!attakState) { 
		//UE_LOG(LogTemp, Warning, TEXT("aaaa"));
		return resultTransform;
	}
	//UE_LOG(LogTemp, Warning, TEXT("llllllllllllll %.2f %.2f %.2f"), pos.X, pos.Y, pos.Z);

	resultTransform = attakState->CalcAttackStateTransformOffset(OwnerAttack, OwnerAttack->StartAngle);
	return resultTransform;
}

FTransform UFloatingWeaponEffect::GetStandStartTransformOffset()
{
	if (!OwnerActor) { return FTransform(); }

	
	return StandStartTransformOffset;
}

void UFloatingWeaponEffect::UpdateTransform()
{
	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::CalcTransformOffset()
{
	if (!OwnerActor) { return; }

	const FVector playerLocation	= OwnerActor->GetActorLocation();
	const FRotator playerRotation	= OwnerActor->GetActorRotation();

	const FVector worldOffsetLocation = playerRotation.RotateVector(LocationOffset);
	const FVector resultLocation = playerLocation + worldOffsetLocation;

	//const FVector resultLocation = playerLocation + LocationOffset;

	Transform.SetLocation(resultLocation);	
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
