// Fill out your copyright notice in the Description page of Project Settings.


#include "FanAttackBase.h"
#include <ProjectNull/Actor/Character/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/Player/PlayerBase.h>
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UFanAttackBase::UFanAttackBase()
	: bRotate(false)
	, Duration(1.0f)
	, ElapsedTime(0.0f)
	, RotationSpeed(1000.0f)
	, Radius(300.0f)
	, ConeAngle(30.0f)
	, bIsActive(false)
	, bPrevActive(false)
	, CurrentAngle(0.0f)
	, KnockbackPower(2.0f)
	, StartAngle(0.0f)
{
}

void UFanAttackBase::Start()
{
	bIsActive		= true;
	CurrentAngle	= StartAngle;
	ElapsedTime		= 0.0f;
}

bool UFanAttackBase::UpdateAttack(float DeltaTime)
{
	if (!bIsActive) { return false; }

	ElapsedTime += DeltaTime;

	// 回転処理
	if (bRotate)
	{
		CurrentAngle += RotationSpeed * DeltaTime;
	}

	// 攻撃範囲をデバッグラインで可視化
	{
		// プレイヤーの座標と前方ベクトルを取得
		const FVector playerLocation = OwnerActor->GetActorLocation();
		const FVector forwardVector = OwnerActor->GetActorForwardVector();

		// 攻撃方向ベクトル
		const FVector attackDir = CalcAttackDir(forwardVector);

		UDebugDrawLibrary::DrawDebugFan(
			GetWorld(),
			playerLocation,
			attackDir,
			Radius,
			ConeAngle,
			10
		);
	}

	// 終了判定
	if (ElapsedTime >= Duration)
	{
		bIsActive = false;
	}

	return true;
}

bool UFanAttackBase::IsTargetInRange(AActor* Target, const FVector& OwnerLocation)
{
	if (!OwnerActor) { return false; }
	if (!Target) { return false; }

	// 敵へのベクトル
	FVector ToEnemy = Target->GetActorLocation() - OwnerLocation;

	// 攻撃方向ベクトル
	const FVector AttackDir = CalcAttackDir(OwnerActor->GetActorForwardVector());

	// 距離チェック
	if (ToEnemy.SizeSquared() > GetRadiusSquared())
	{
		return false;
	}

	// ベクトル正規化
	ToEnemy.Normalize();

	// 角度チェック
	float Dot = FVector::DotProduct(AttackDir, ToEnemy);

	return Dot > GetConeCosine();
}


void UFanAttackBase::AttackJudgePlayer(APlayerBase* Player)
{
	if (!Player) { return; }

	// 持ち主の座標を取得
	const FVector ownerLocation = OwnerActor->GetActorLocation();

	// 敵が攻撃範囲内にいるか判定
	if (IsTargetInRange(Player, ownerLocation))
	{
		// ダメージを与える(未実装)
	}
}

void UFanAttackBase::AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager)
{
	if (!EnemyManager) { return; }

	// プレイヤーの座標を取得
	const FVector playerLocation = OwnerActor->GetActorLocation();

	// 敵リストをループして、攻撃範囲内の敵にダメージを与える
	for (auto& enemy : EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		// 敵が攻撃範囲内にいるか判定
		if (IsTargetInRange(enemy, playerLocation))
		{
			// 第二引数※要改善(ノックバックの強さなどは基底に置くかゲットできるようにしたい)
			enemy->SetKnockBackData(playerLocation, 2.0f, 1.0f);

			// ダメージを与える
			enemy->SetTakeDamaged(10);
		}
	}
}

bool UFanAttackBase::CanDeactivate()
{
	bool canDeactivate = (bIsActive != bPrevActive) && !bIsActive;
	bPrevActive = bIsActive;
	return canDeactivate;
}

bool UFanAttackBase::IsActiveFirstFrame()
{
	bool canDeactivate = (bIsActive != bPrevActive) && bIsActive;
	bPrevActive = bIsActive;
	return canDeactivate;
}

void UFanAttackBase::UpdatePrevActiveFlg()
{
	bPrevActive = bIsActive;
}

FVector UFanAttackBase::CalcAttackDir(const FVector& forwardVector) const
{
	const float angle = bRotate ? CurrentAngle : 0.0f;
	return forwardVector.RotateAngleAxis(angle, FVector::UpVector);
}