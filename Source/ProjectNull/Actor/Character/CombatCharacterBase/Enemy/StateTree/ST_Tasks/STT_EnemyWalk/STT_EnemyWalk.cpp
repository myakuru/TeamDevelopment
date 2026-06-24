#include "STT_EnemyWalk.h"
#include "StateTreeExecutionContext.h"
#include <ProjectNull\Actor\Character\CombatCharacterBase\Enemy\EnemyBase.h>
#include <ProjectNull/Data/CharacterRuntimeData/EnemyRuntimeData/EnemyRuntimeData.h>

USTT_EnemyWalk::USTT_EnemyWalk(const FObjectInitializer& a_ObjInit)
	:	Super(a_ObjInit)
	,	MoveDir(FVector::ZeroVector)
	,	GravityVelocity(FVector::ZeroVector)
	,	AfterGroundNormal(FVector::ZeroVector)
	,	MoveSpeed(600.f)
	,	RotationInterpSpeed(5.f)
	,	WarkableFloorAngle(45.f)
	,	MaxStepHeight(100.f)
	,	CapsuleHalfHeight(88.f)
	,	isGround(false)
{
	// Tick処理有効化	
	bShouldCallTick = true;
}

EStateTreeRunStatus USTT_EnemyWalk::EnterState(FStateTreeExecutionContext& a_Context, const FStateTreeTransitionResult& a_Transition)
{
	Super::EnterState(a_Context, a_Transition);

	OwnerEnemy = Cast<AEnemyBase>(a_Context.GetOwner());
	if (!OwnerEnemy) { return EStateTreeRunStatus::Failed; }

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
	_cacheMoveVec.Z = 0.f;
	_cacheMoveVec = FVector::VectorPlaneProject(_cacheMoveVec, AfterGroundNormal);	// 坂道に沿った移動ベクトルに変換
	_cacheMoveVec += GravityVelocity * a_DeltaTime;									// 重力適応

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
			
			// 上向き法線が一定以上なら坂道とみなす
			if (_hit.ImpactNormal.Z >= FMath::Cos(WarkableFloorAngle))
			{
				// 坂道処理
				MoveSlope(_hit);
				_cacheMoveVec = FVector::VectorPlaneProject(CaluculateRemainingMoveDir(_cacheMoveVec, _hit.Time), _hit.ImpactNormal);
				OwnerEnemy->AddActorWorldOffset(_cacheMoveVec);
			}
			// 上向き法線が一定以下で、段差の高さが一定以下なら段差とみなす
			else if (FMath::IsNearlyZero(_hit.ImpactNormal.Z) && _heightDiff < MaxStepHeight)
			{
				// 段差処理
				MoveStep(_hit, _cacheMoveVec, _heightDiff);
			}
			// それ以外は壁とみなす
			else
			{
				isGround = true;
				// 急斜面移動
				// 直接上に速度半分で移動させる(壁の法線に沿って移動させると、凹凸に応じてクネる為)
				OwnerEnemy->AddActorWorldOffset((FVector::UpVector * MoveSpeed * a_DeltaTime));
				{
					// 壁の法線に沿って移動する処理(法線をそのまま使う為、凹凸に応じてクネる)
					/*FVector SurfaceMove =
						FVector::VectorPlaneProject(
							OwnerEnemy->GetActorForwardVector(),
							_hit.ImpactNormal).GetSafeNormal();

					OwnerEnemy->AddActorWorldOffset(CaluculateRemainingMoveDir(SurfaceMove * MoveSpeed * a_DeltaTime, _hit.Time));*/
				}
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
	OwnerEnemy->AddActorWorldOffset(CaluculateRemainingMoveDir(a_MoveVec, a_HitResult.Time),true);
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