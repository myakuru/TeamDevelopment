#include "STT_EnemyWalk.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

USTT_EnemyWalk::USTT_EnemyWalk(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
	, MoveDir(FVector::ZeroVector)
{
	// Tick処理有効化	
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyWalk::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	InitializeWalkParams();

	return EStateTreeRunStatus::Running;
}

EStateTreeRunStatus USTT_EnemyWalk::Tick(FStateTreeExecutionContext& a_Context, const float a_DeltaTime)
{
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

	Super::Tick(a_Context, a_DeltaTime);

	Move(a_DeltaTime);

	return EStateTreeRunStatus::Running;
}

void USTT_EnemyWalk::ExitState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::ExitState(a_Context, a_Transition);
}

void USTT_EnemyWalk::InitializeWalkParams()
{

}

void USTT_EnemyWalk::Move(const float a_DeltaTime)
{
	if (!OwnerEnemy) { return; }

	// 仮でマジックナンバーにしてるがゲッターを別途用意
	static float _RotSpeed = 5.0f;
	static float _MoveSpeed = 300.0f;

	// 回転角度補間
	FRotator calcResultRot = CalculateRotationToMoveDir(
		OwnerEnemy->GetActorRotation(),
		MoveDir.Rotation(),
		a_DeltaTime,
		_RotSpeed
	);
	// Yaw起点でしか回転しない
	calcResultRot.Roll = calcResultRot.Pitch = 0;
	
	// 角度更新
	OwnerEnemy->SetActorRotation(calcResultRot);

	// 座標更新
	GravityVelocity += FVector(0.f, 0.f, GetWorld()->GetGravityZ()) * a_DeltaTime;
	if (isGround)
	{
		GravityVelocity = FVector::ZeroVector;
	}

	MoveDir.Z = 0;
	FVector _delta = MoveDir * _MoveSpeed * a_DeltaTime;
	_delta = FVector::VectorPlaneProject(_delta, AfterGroundNormal);
	_delta += GravityVelocity * a_DeltaTime;
	FHitResult _hit;
	OwnerEnemy->AddActorWorldOffset(
		_delta,
		true,
		&_hit);

	if (_hit.bBlockingHit)
	{
		isGround = false;
		const float _walkableAngle = FMath::Cos(FMath::DegreesToRadians(45.f));
		const float _heightDiff = _hit.ImpactPoint.Z - (OwnerEnemy->GetActorLocation().Z-88.0f/*カプセルの高さ半径分*/);

		// 上向き法線が無くて、段差の高さが一定以下なら段差を乗り越える
		if ((FMath::IsNearlyZero(_hit.ImpactNormal.Z)) &&
			_heightDiff <= 100.f)
		{
			isGround = true;
			OwnerEnemy->AddActorWorldOffset(FVector(0, 0, _heightDiff));

			// 衝突して使われなかった分の移動量を算出
			FVector _remaining = _delta * (1.0f - _hit.Time);

			// 進行ベクトルの内、衝突ベクトルだけ除去して補正
			OwnerEnemy->AddActorWorldOffset(_remaining);
			UE_LOG(LogTemp, Warning, TEXT("Diff : %f"), _heightDiff);
			DrawDebugPoint(GetWorld(), _hit.ImpactPoint, 5.0f, FColor::Red, false, 2.f);
		}
		else if (_hit.ImpactNormal.Z >= _walkableAngle)
		{
			isGround = true;
			AfterGroundNormal = _hit.ImpactNormal;
			// 衝突して使われなかった分の移動量を算出
			FVector _remaining = _delta * (1.0f - _hit.Time);

			// 進行ベクトルの内、衝突ベクトルだけ除去して補正
			FVector _adjust = FVector::VectorPlaneProject(_remaining, _hit.ImpactNormal);
			OwnerEnemy->AddActorWorldOffset(_adjust);
		}
	}
	else
	{
		isGround = false;
	}
}

void USTT_EnemyWalk::MoveSlope()
{
	
}

FVector USTT_EnemyWalk::CalculateNextActorLocation(const FVector& a_CurrentLocation, const FVector& a_MoveDir, float a_MoveSpeed, float a_DeltaTime)
{
	return a_CurrentLocation + a_MoveDir * a_MoveSpeed * a_DeltaTime;
}

FRotator USTT_EnemyWalk::CalculateRotationToMoveDir(const FRotator& a_CurrentRot, const FRotator& a_TargetRot, float a_DeltaTime, float a_RotSpeed)
{
	return FMath::RInterpTo(
						a_CurrentRot,
						a_TargetRot,
						a_DeltaTime,
						a_RotSpeed);
}