#include "FanAttackBase.h"

#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/Utility/Common/Definitions/CollisionChannels.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>

UFanAttackBase::UFanAttackBase():
		bRotate(false),
		Duration(1.0f),
		ElapsedTime(0.0f),
		RotationSpeed(1000.0f),
		Radius(300.0f),
		ConeAngle(30.0f),
		bPrevActive(false),
		CurrentAngle(0.0f),
		KnockbackPower(2.0f),
		StartAngle(0.0f),
		bIsDrawDebugLine(false)
{
}

void UFanAttackBase::Start()
{
	bIsActive		= true;
	CurrentAngle	= StartAngle;
	ElapsedTime		= 0.0f;
}

void UFanAttackBase::Execute()
{
}

void UFanAttackBase::Update(float DeltaTime)
{
	UAttackBase::Update(DeltaTime);

	UpdateAttack(DeltaTime);
}

bool UFanAttackBase::UpdateAttack(float DeltaTime)
{
	if (!bIsActive || !RootComponent || !RootComponent->GetAttachParent()) { return false; }

	ElapsedTime += DeltaTime;

	// 回転処理
	if (bRotate) {
		CurrentAngle += RotationSpeed * DeltaTime;
	}

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

void UFanAttackBase::AttackJudge()
{
	if (!OwnerActor) { return; }

	if (auto* root = OwnerActor->GetRootComponent())
	{
		ECollisionChannel collisionChannel =
			root->GetCollisionObjectType();

		// コリジョンタイプが「Pawn(プレイヤー)」か
		if (collisionChannel == ECC_Player)
		{
			auto* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
			if (!enemyManager) { return; }

			AttackJudgeEnemys(enemyManager);
		}
		else if (collisionChannel == ECC_Enemy)
		{
			auto* playerPawn = UGameplayStatics::GetPlayerPawn(this, 0);
			if (!playerPawn) { return; }

			AttackJudgePlayer(playerPawn);
		}
	}
}

void UFanAttackBase::AttackJudgePlayer(const TObjectPtr<AActor>& a_Player)
{
	if (!a_Player|| !bIsActive) { return; }

	// 持ち主の座標を取得
	const FVector ownerLocation = OwnerActor->GetActorLocation();

	// 敵が攻撃範囲内にいるか判定
	if (IsTargetInRange(a_Player))
	{
		// ダメージを与える(未実装)
		if (auto* interface = Cast<ICharacterInterface>(a_Player))
		{
			interface->TakeDamaged();
			interface->TakeKnockBack(ownerLocation);
		}
	}
}

void UFanAttackBase::AttackJudgeEnemys(const TObjectPtr<UEnemyManagerSubsystem>& a_EnemyManager)
{
	if (!a_EnemyManager||!bIsActive) { return; }

	// プレイヤーの座標を取得
	const FVector location = OwnerActor->GetActorLocation();

	// 敵リストをループして、攻撃範囲内の敵にダメージを与える
	for (auto& enemy : a_EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		// 敵が攻撃範囲内にいるか判定
		if (IsTargetInRange(enemy))
		{
			if (auto* interface = Cast<ICharacterInterface>(enemy))
			{
				interface->TakeDamaged();
				interface->TakeKnockBack(location);
			}
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
