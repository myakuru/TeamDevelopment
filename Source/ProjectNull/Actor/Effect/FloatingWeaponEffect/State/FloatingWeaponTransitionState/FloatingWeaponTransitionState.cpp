

#include "FloatingWeaponTransitionState.h"

#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FloatingWeaponAttack/FloatingWeaponAttack.h>


UFloatingWeaponTransitionState::UFloatingWeaponTransitionState():
	NextState(EFloatingWeaponState::Attack),
	StartLocationOffset(FVector::ZeroVector),
	StartRotationOffset(FQuat()),
	TargetTransform(FTransform())
{
}

void UFloatingWeaponTransitionState::Start(EFloatingWeaponState SetNextState)
{
	NextState		= SetNextState;
	TransitionTime	= GetTransitionStateTime();

	if (!Owner) { return; }
	//オフセットがほしい
	StartLocationOffset = Owner->GetLocationOffset();
	StartRotationOffset = Owner->GetTransform().GetRotation();

	// 遷移後の状態に応じて、補間先のTransform情報を変更
	if (NextState == EFloatingWeaponState::Attack)
	{
		TargetTransform = Owner->GetAttackStartTransformOffset();
	}
	else if (NextState == EFloatingWeaponState::Stand)
	{
		TargetTransform = Owner->GetStandStartTransformOffset();
	}

}

void UFloatingWeaponTransitionState::Update(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	UE_LOG(LogTemp, Warning, TEXT("TransitionState"));

	UpdateTransitionTime(DeltaTime);
	
	UpdateTransformOffsetLerp(DeltaTime);
	
	UFloatingWeaponStateBase::Update(DeltaTime);
}

void UFloatingWeaponTransitionState::UpdateTransformOffsetLerp(float DeltaTime)
{
	if (!OwnerActor || !Owner || !Owner->GetOwnerAttack()) { return; }

	// オフセットの補間→最終的にプレイヤー座標も考慮して計算
	float lerpValue = 1.0f - (TransitionTime / GetTransitionStateTime());
	lerpValue = std::clamp(lerpValue, 0.0f, 1.0f);

	// 補間処理
	const FVector resultLocation	= FMath::Lerp(StartLocationOffset, TargetTransform.GetLocation(), lerpValue);
	const FQuat4d resultQuaternion	= FQuat4d::Slerp(StartRotationOffset, TargetTransform.GetRotation(), lerpValue);

	// オフセットTransform更新
	LocationOffset = resultLocation;
	//Owner->SetRotation(resultQuaternion.Rotator());

	// 状態遷移処理
	switch (NextState)
	{
	case EFloatingWeaponState::Stand:	TryChangeToStandState();	break;
	case EFloatingWeaponState::Attack:	TryChangeToAttackState();	break;
	case EFloatingWeaponState::Transition:	return;
	case EFloatingWeaponState::Count:		return;
	default: return;
	}
}

void UFloatingWeaponTransitionState::TryChangeToStandState()
{
	if (!Owner) { return; }

	if (IsFinishedTransitionState())
	{
		Owner->ChangeState(EFloatingWeaponState::Stand);
		return;
	}
}

void UFloatingWeaponTransitionState::TryChangeToAttackState()
{
	if (!Owner || !Owner->GetOwnerAttack()) { return; }

	if (Owner->GetOwnerAttack()->IsAttackStateStep())
	{
		Owner->ChangeState(EFloatingWeaponState::Attack);
		return;
	}	
}
