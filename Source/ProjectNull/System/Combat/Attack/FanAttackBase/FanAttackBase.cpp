#include "FanAttackBase.h"

#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
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
	, StartAngle(0.0f),
	bIsDrawDebugLine(false)
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
	if (!bIsActive || !RootComponent || !RootComponent->GetAttachParent()) { return false; }

	ElapsedTime += DeltaTime;

	// 回転処理
	if (bRotate) {
		CurrentAngle += RotationSpeed * DeltaTime;
	}
	//UE_LOG(LogTemp, Warning, TEXT("StartAngle %.2f"), StartAngle);

	// 攻撃範囲をデバッグラインで可視化
	{
		// プレイヤーの座標と前方ベクトルを取得
		const FVector location = RootComponent->GetComponentLocation();
		const FVector forwardVector = RootComponent->GetForwardVector();

		// 攻撃方向ベクトル
		const FVector attackDir = CalcAttackDir(forwardVector);

		if (bIsDrawDebugLine)
		{
			UDebugDrawLibrary::DrawDebugFan(
				GetWorld(),
				location,
				attackDir,
				Radius,
				ConeAngle,
				10
			);
		}
	}

	// 終了判定
	if (ElapsedTime >= Duration) {
		bIsActive = false;
	}

	return true;
}

bool UFanAttackBase::IsTargetInRange(AActor* Target)
{
	if (!OwnerActor || !Target || !RootComponent || !RootComponent->GetAttachParent())	{ return false; }
	const auto* parent = RootComponent->GetAttachParent();

	// 敵へのベクトル
	FVector toEnemy = Target->GetActorLocation() - parent->GetComponentLocation();

	// 攻撃方向ベクトル
	const FVector attackDir = CalcAttackDir(parent->GetForwardVector());

	// 距離チェック
	if (toEnemy.SizeSquared() > GetRadiusSquared()) {
		return false;
	}

	// ベクトル正規化
	toEnemy.Normalize();

	// 角度チェック
	float dot = FVector::DotProduct(attackDir, toEnemy);

	return dot > GetConeCosine();
}


void UFanAttackBase::AttackJudgePlayer(AActor* Player)
{
	if (!Player) { return; }

	// 持ち主の座標を取得
	const FVector ownerLocation = OwnerActor->GetActorLocation();

	// 敵が攻撃範囲内にいるか判定
	if (IsTargetInRange(Player))
	{
		// ダメージを与える(未実装)
	}
}

void UFanAttackBase::AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager)
{
	if (!EnemyManager) { return; }

	// プレイヤーの座標を取得
	const FVector location = OwnerActor->GetActorLocation();

	// 敵リストをループして、攻撃範囲内の敵にダメージを与える
	for (auto& enemy : EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		// 敵が攻撃範囲内にいるか判定
		if (IsTargetInRange(enemy))
		{
			// 第二引数※要改善(ノックバックの強さなどは基底に置くかゲットできるようにしたい)
			enemy->SetKnockBackData(location, 2.0f, 1.0f);

			// ダメージを与える
			enemy->SetTakeDamaged();
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

FVector UFanAttackBase::CalcAttackDir(const FVector& forwardVector, float Angle) const
{
	const float angle = bRotate ? Angle : 0.0f;
	return forwardVector.RotateAngleAxis(angle, FVector::UpVector);
}
