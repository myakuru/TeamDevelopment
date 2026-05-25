#include "STT_EnemyWalk.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>

USTT_EnemyWalk::USTT_EnemyWalk(const FObjectInitializer& a_ObjInit)
	: Super(a_ObjInit)
	, MoveDir(FVector::ZeroVector)
	, AfterGroundNormal(FVector::ZeroVector)
	, GravityVelocity(FVector::ZeroVector)
	, isGround(false)
{
	// Tick処理有効化	
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyWalk::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());

	// パラメータの初期化
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

void USTT_EnemyWalk::Move(const float a_DeltaTime)
{
	if (!OwnerEnemy) { return; }

	// 回転角度補間
	FRotator calcResultRot = CalculateRotationToMoveDir(
		OwnerEnemy->GetActorRotation(),
		MoveDir.Rotation(),
		a_DeltaTime,
		RotationInterpSpeed
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

	FVector _cacheMoveVec = MoveDir * MoveSpeed * a_DeltaTime;
	_cacheMoveVec.Z = 0;												// Z成分は考慮しない
	_cacheMoveVec 
		= FVector::VectorPlaneProject(_cacheMoveVec, AfterGroundNormal);// 前に触れた地面の斜面に沿わせる
	_cacheMoveVec += GravityVelocity * a_DeltaTime;						// 重力適応	

	FHitResult _hit;
	OwnerEnemy->AddActorWorldOffset(
		_cacheMoveVec,
		true,
		&_hit);

	// Hitしているか
	if (_hit.bBlockingHit)
	{
		// コリジョンチャネルが「WorldStatic」(マップオブジェクト類)なら座標補正を行う
		if (_hit.GetComponent() &&
			_hit.GetComponent()->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic)
		{
			isGround = false;
			const float _heightDiff
				= _hit.ImpactPoint.Z - (OwnerEnemy->GetActorLocation().Z - CapsuleHalfHeight);

			// 上向き法線が無くて、段差の高さが一定以下なら段差を乗り越える
			if ((FMath::IsNearlyZero(_hit.ImpactNormal.Z)) &&
				_heightDiff <= MaxStepHeight)
			{
				// 段差移動
				MoveStep(_hit, _cacheMoveVec, _heightDiff);
			}
			// 上向き法線が一定以上なら坂道とみなす
			else if (_hit.ImpactNormal.Z >= FMath::Cos(WarkableFloorAngle))
			{
				// 坂道処理
				MoveSlope(_hit);
			}
		}
	}
	else
	{
		isGround = false;
	}
}

void USTT_EnemyWalk::MoveSlope(const FHitResult& a_HitResult)
{
	isGround = true;
	AfterGroundNormal = a_HitResult.ImpactNormal;
}

void USTT_EnemyWalk::MoveStep(const FHitResult& a_HitResult, const FVector& a_MoveVec, const float a_HeightDiff)
{
	if (!OwnerEnemy) { return; }

	isGround = true;
	OwnerEnemy->AddActorWorldOffset(
		FVector(0, 0, a_HeightDiff) + CaluculateRemainingMoveDir(a_MoveVec, a_HitResult.Time));
}

FVector USTT_EnemyWalk::CaluculateRemainingMoveDir(const FVector& a_DeltaMoveDir, const float a_HitTime)
{
	return a_DeltaMoveDir * (1.0f - a_HitTime);
}

FRotator USTT_EnemyWalk::CalculateRotationToMoveDir(const FRotator& a_CurrentRot, const FRotator& a_TargetRot, float a_DeltaTime, float a_RotSpeed)
{
	return FMath::RInterpTo(
						a_CurrentRot,
						a_TargetRot,
						a_DeltaTime,
						a_RotSpeed);
}

void USTT_EnemyWalk::InitializeWalkParams()
{
	// ラジアン角に補正
	WarkableFloorAngle = FMath::DegreesToRadians(WarkableFloorAngle);
}