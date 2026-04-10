#include "AutoAttack.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/Actor/Character/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/Player/PlayerBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h>
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>



UAutoAttack::UAutoAttack()
	: AutoAttackInterval(5.0f)
	, FrontToRingDelay(1.0f)
{
	
}

void UAutoAttack::Initialize(AActor* Owner)
{
	UAttackBase::Initialize(Owner);


	if (FloatingWeaponMap.Contains(EAutoAttackType::Front))
	{
		if (auto* floatingWeapon = FloatingWeaponMap[EAutoAttackType::Front])
		{
			floatingWeapon->SetOwnerAttack(AutoAttackParamsMap[EAutoAttackType::Front]);
			floatingWeapon->Start(OwnerActor->GetRootComponent());
		}
	}

	if (FloatingWeaponMap.Contains(EAutoAttackType::Ring))
	{
		if (auto* floatingWeapon = FloatingWeaponMap[EAutoAttackType::Ring])
		{
			floatingWeapon->SetOwnerAttack(AutoAttackParamsMap[EAutoAttackType::Ring]);
		}
	}


	for (auto& [type, floatingWeapon] : FloatingWeaponMap)
	{
		if (!floatingWeapon) { continue; }
		floatingWeapon->Initialize();
	}

	//　自動攻撃のタイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		AutoFrontConeAttackTimerHandle,
		this,
		&UAutoAttack::StartAutoAttack,
		AutoAttackInterval,
		true);


	/*if (FloatingWeaponMap.Contains(EAutoAttackType::Front)
		&& FloatingWeaponMap[EAutoAttackType::Front])
	{
		FloatingWeaponMap[EAutoAttackType::Front]->Start(OwnerActor->GetRootComponent());
	}*/
}

void UAutoAttack::Execute()
{
	return;
}

void UAutoAttack::Update(float DeltaTime)
{
	//　敵管理クラスの情報取得
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	//　自動攻撃の更新
	for(auto& [type,coneSlashParams] : AutoAttackParamsMap)
	{
		if (!coneSlashParams) { continue; }

		UpdateAutoAttack(DeltaTime, *coneSlashParams, enemyManager);
	}

	//　浮遊武器の更新
	for (auto& [type, floatingWeapon] : FloatingWeaponMap)
	{
		if (!floatingWeapon) { continue; }

		floatingWeapon->Update(OwnerActor,DeltaTime);
	}

}

void UAutoAttack::StartAutoAttack()
{
	if (!OwnerActor) { return; }

	if(AutoAttackParamsMap.Contains(EAutoAttackType::Front)
		&& AutoAttackParamsMap[EAutoAttackType::Front])
	{
		AutoAttackParamsMap[EAutoAttackType::Front]->Start();	
	}

	
	

	//　前方扇状自動攻撃からの周囲攻撃遅延タイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		FrontToRingDelayTimerHandle,
		this,
		&UAutoAttack::StartAutoRingAttack,
		FrontToRingDelay,
		false);
}

void UAutoAttack::StartAutoRingAttack()
{
	if (FloatingWeaponMap.Contains(EAutoAttackType::Ring)
		&& FloatingWeaponMap[EAutoAttackType::Ring])
	{
		FloatingWeaponMap[EAutoAttackType::Ring]->Start(OwnerActor->GetRootComponent());
	}
}
void UAutoAttack::UpdateAutoAttack(float DeltaTime, URingPulseSlashAttack& RingPulseSlashAttack, UEnemyManagerSubsystem* EnemyManager)
{
	if (!OwnerActor)	{ return; }
	if (!EnemyManager)	{ return; }


	if (!RingPulseSlashAttack.UpdateAttack(DeltaTime)) { return; }

	//　プレイヤーの座標と前方ベクトルを取得
	const FVector playerLocation	= OwnerActor->GetActorLocation();
	const FVector forwardVector		= OwnerActor->GetActorForwardVector();

	//　攻撃方向ベクトル
	const FVector attackDir = RingPulseSlashAttack.CalcAttackDir(forwardVector);

	{
		//　攻撃範囲をデバッグラインで可視化
		UDebugDrawLibrary::DrawDebugFan
		(
			GetWorld(),
			playerLocation,
			attackDir,
			RingPulseSlashAttack.Radius,
			RingPulseSlashAttack.ConeAngle,
			10
		);
	}

	//　敵リストをループして、攻撃範囲内の敵にダメージを与える
	for (auto& enemy : EnemyManager->GetEnemyList())
	{
		if (!enemy) { continue; }

		//　敵が扇範囲内にいるか判定
		if (IsEnemyInConeRange(enemy, playerLocation, attackDir, RingPulseSlashAttack))
		{
			enemy->SetKnockBackData(playerLocation, RingPulseSlashAttack.KnockbackPower,1.0f);
			enemy->SetTakeDamaged(10);
		}
	}
}

bool UAutoAttack::IsEnemyInConeRange(AActor* Enemy, const FVector& PlayerLocation, const FVector& AttackDir, const URingPulseSlashAttack& RingPulseSlashAttack) const
{
	if (!Enemy) { return false; }

	//　敵へのベクトル
	FVector toEnemy = Enemy->GetActorLocation() - PlayerLocation;

	//　距離チェック
	if (toEnemy.SizeSquared() > RingPulseSlashAttack.GetRadiusSquared())
	{
		return false;
	}

	//　ベクトル正規化
	toEnemy.Normalize();

	//　角度チェック
	const float dot = FVector::DotProduct(AttackDir, toEnemy);

	return dot > RingPulseSlashAttack.GetConeCosine();
}

