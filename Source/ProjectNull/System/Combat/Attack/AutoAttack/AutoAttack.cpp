#include "AutoAttack.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/Actor/Character/Enemy/EnemyBase.h>
#include <ProjectNull/Actor/Character/Player/PlayerBase.h>
#include <ProjectNull/Utility/DebugDrawLibrary/DebugDrawLibrary.h>
#include <ProjectNull/System/Combat/Attack/FanAttackBase/FanAttackBase.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>



UAutoAttack::UAutoAttack()
	: AutoAttackInterval(5.0f)
	, FrontToRingDelay(1.0f)
{
}

void UAutoAttack::Initialize(AActor* Owner)
{
	UAttackBase::Initialize(Owner);

	// 自動攻撃のパラメータクラスを初期化
	for (auto& [type, ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }

		ConeSlashParams->Initialize(Owner);
	}

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

void UAutoAttack::Update(float DeltaTime, APlayerBase* Player, UEnemyManagerSubsystem* EnemyManager)
{
	if (!EnemyManager) { return; }

	for (auto& [type, ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }

		if (!ConeSlashParams->UpdateAttack(DeltaTime)) { continue; }

		ConeSlashParams->AttackJudge(Player, EnemyManager);
	}

	// 
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