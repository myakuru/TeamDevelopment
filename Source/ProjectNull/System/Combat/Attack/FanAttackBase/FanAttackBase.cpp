#include "FanAttackBase.h"

#include "Kismet/GameplayStatics.h"
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/Utility/GroundUtility/GroundUtility.h>
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
	SetIsActive(true);
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
	if (!IsActive())	{ return false; }

	auto RootComp		= GetRootComponent();
	if (!RootComp)		{ return false; }

	auto AttachParent	= RootComp->GetAttachParent();
	if (!AttachParent)	{ return false; }


	ElapsedTime += DeltaTime;

	// 回転処理
	if (bRotate) {
		CurrentAngle += RotationSpeed * DeltaTime;
	}

	// 攻撃範囲をデバッグラインで可視化
	{
		// プレイヤーの座標と前方ベクトルを取得
		const FVector Location		= RootComp->GetComponentLocation();
		const FVector ForwardVector = RootComp->GetForwardVector();

		// 攻撃方向ベクトル
		const FVector AttackDir = CalcAttackDir(ForwardVector);

		if (bIsDrawDebugLine)
		{
			UDebugDrawLibrary::DrawDebugFan(
				GetWorld(),
				Location,
				AttackDir,
				Radius,
				ConeAngle,
				10
			);
		}
	}

	// 終了判定
	if (ElapsedTime >= Duration) {
		SetIsActive(false);
	}

	return true;
}

bool UFanAttackBase::IsTargetInRange(AActor* Target)
{
	if (!GetOwnerActor() ||
		!Target ||
		!GetRootComponent()||
		!GetRootComponent()->GetAttachParent()) { return false; }
	const auto* parent = GetRootComponent()->GetAttachParent();

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
	if (!GetOwnerActor()) { return; }

	if (auto* root = GetOwnerActor()->GetRootComponent())
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
	if (!a_Player|| !IsActive()) { return; }

	// 持ち主の座標を取得
	const FVector ownerLocation = GetOwnerActor()->GetActorLocation();

	// 敵が攻撃範囲内にいるか判定
	if (IsTargetInRange(a_Player))
	{
		// ダメージを与える(未実装)
		if (auto* interface = Cast<ICharacterInterface>(a_Player))
		{
			interface->ApplyDamaged();
			interface->ApplyKnockBack(ownerLocation);
		}
	}
}

void UFanAttackBase::AttackJudgeEnemys(const TObjectPtr<UEnemyManagerSubsystem>& a_EnemyManager)
{
	if (!a_EnemyManager||!IsActive()) { return; }

	// プレイヤーの座標を取得
	const FVector location = GetOwnerActor()->GetActorLocation();

	// 敵リストをループして、攻撃範囲内の敵にダメージを与える
	for (auto& enemy : a_EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		// 敵が攻撃範囲内にいるか判定
		if (IsTargetInRange(enemy))
		{
			if (auto* interface = Cast<ICharacterInterface>(enemy))
			{
				interface->ApplyDamaged();
				interface->ApplyKnockBack(location);
			}
		}
	}
}

bool UFanAttackBase::CanDeactivate()
{
	const bool bCurrentActive = IsActive();
	bool canDeactivate = (bCurrentActive != bPrevActive) && !bCurrentActive;
	bPrevActive = bCurrentActive;
	return canDeactivate;
}

bool UFanAttackBase::IsActiveFirstFrame()
{
	const bool bCurrentActive = IsActive();
	bool canDeactivate = (bCurrentActive != bPrevActive) && bCurrentActive;
	bPrevActive = bCurrentActive;
	bPrevActive = bCurrentActive;
	return canDeactivate;
}

void UFanAttackBase::UpdatePrevActiveFlg()
{
	bPrevActive = IsActive();
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

void UFanAttackBase::UpdateRotation(float DeltaTime)
{
	if (!AbsoluteRotation())	{ return; }

	auto RootComp	= GetRootComponent();
	if (!RootComp)				{ return; }

	auto Player		= Cast<APlayerBase>(GetOwnerActor());
	if (!Player)				{ return; }

	FVector FloorNormal = FVector::ZeroVector;
	if (!Player->GetCurrentFloorNormal(FloorNormal)) { return; }

	const FQuat TargetQuat = UGroundUtility::MakeRotationFromGroundNormal(
		RootComp->GetComponentTransform(),
		FloorNormal);

	const FQuat NewQuat = FQuat::Slerp(
		RootComp->GetComponentQuat(),
		TargetQuat,
		DeltaTime * RotationInterpSpeed);

	RootComp->SetWorldRotation(NewQuat);
}
