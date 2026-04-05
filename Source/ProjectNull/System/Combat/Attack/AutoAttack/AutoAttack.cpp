#include "AutoAttack.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include "ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h"
#include "ProjectNull/Actor/Character/Enemy/EnemyBase.h"
#include "ProjectNull/Actor/Character/Player/PlayerBase.h"
#include "ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h"
#include "ProjectNull/System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h"


UAutoAttack::UAutoAttack()
	: AutoAttackInterval(5.0f)
	, FrontToRingDelay(1.0f)
{
	
}

void UAutoAttack::Initialize(AActor* Owner)
{
	UAttackBase::Initialize(Owner);

	//　自動攻撃のタイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		AutoFrontConeAttackTimerHandle,
		this,
		&UAutoAttack::StartAutoAttack,
		AutoAttackInterval,
		true);

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


	for(auto& [type,ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }

		UpdateAutoAttack(DeltaTime, *ConeSlashParams, enemyManager);
	}

}

void UAutoAttack::StartAutoAttack()
{
	if (!OwnerActor) { return; }

	if(AutoAttackParamsMap.Contains(EAutoAttackType::Front) && AutoAttackParamsMap[EAutoAttackType::Front]){

		AutoAttackParamsMap[EAutoAttackType::Front]->Start();

		if (AutoFrontEffect) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				AutoFrontEffect,
				OwnerActor->GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}
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
	if (AutoAttackParamsMap.Contains(EAutoAttackType::Ring) && AutoAttackParamsMap[EAutoAttackType::Ring]) {
		AutoAttackParamsMap[EAutoAttackType::Ring]->Start();

		if (AutoRingEffect) {
			UNiagaraFunctionLibrary::SpawnSystemAttached(
				AutoRingEffect,
				OwnerActor->GetRootComponent(),
				NAME_None,
				FVector::ZeroVector,
				FRotator::ZeroRotator,
				EAttachLocation::KeepRelativeOffset,
				true
			);
		}

	}
}
void UAutoAttack::UpdateAutoAttack(float DeltaTime, URingPulseSlashAttack& RingPulseSlashAttack, UEnemyManagerSubsystem* EnemyManager)
{
	if (!OwnerActor) { return; }
	if (!EnemyManager) { return; }


	if (!RingPulseSlashAttack.UpdateAttack(DeltaTime)) { return; }

	//　プレイヤーの座標と前方ベクトルを取得
	const FVector playerLocation = OwnerActor->GetActorLocation();
	const FVector forwardVector = OwnerActor->GetActorForwardVector();

	//　攻撃方向ベクトル
	const FVector attackDir = RingPulseSlashAttack.CalcAttackDir(forwardVector);

	{
		//　攻撃範囲をデバッグラインで可視化
		UDebugDrawLibrary::DrawDebugFan(
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
			enemy->OnDeath();
		}
	}
}

bool UAutoAttack::IsEnemyInConeRange(AActor* Enemy, const FVector& PlayerLocation, const FVector& AttackDir, const URingPulseSlashAttack& RingPulseSlashAttack) const
{
	if (!Enemy) { return false; }

	//　敵へのベクトル
	FVector ToEnemy = Enemy->GetActorLocation() - PlayerLocation;

	//　距離チェック
	if (ToEnemy.SizeSquared() > RingPulseSlashAttack.GetRadiusSquared())
	{
		return false;
	}

	//　ベクトル正規化
	ToEnemy.Normalize();

	//　角度チェック
	float Dot = FVector::DotProduct(AttackDir, ToEnemy);

	return Dot > RingPulseSlashAttack.GetConeCosine();
}