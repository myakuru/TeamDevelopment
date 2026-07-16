#include "AutoAttack.h"

#include <ProjectNull/System/Combat/Attack/FloatingWeaponAttack/FloatingWeaponAttack.h>



UAutoAttack::UAutoAttack():
	AutoAttackInterval(5.0f),
	FrontToRingDelay(1.0f)
{
}

void UAutoAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	UAttackBase::Initialize(Owner);

	// 自動攻撃のパラメータクラスを初期化
	for (auto& [Type, ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }

		ConeSlashParams->SetAutoAttack(this);
		ConeSlashParams->Initialize(Owner);
	}

	if(AutoAttackParamsMap.Contains(EAutoAttackType::Front)
			&& AutoAttackParamsMap[EAutoAttackType::Front])
	{
		AutoAttackParamsMap[EAutoAttackType::Front]->Execute();
	}

	if (AutoAttackParamsMap.Contains(EAutoAttackType::Ring)
			&& AutoAttackParamsMap[EAutoAttackType::Ring])
	{
		AutoAttackParamsMap[EAutoAttackType::Ring]->Execute();
	}
	
	// 自動攻撃のタイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		AutoFrontConeAttackTimerHandle,
		this,
		&UAutoAttack::StartAutoAttack,
		AutoAttackInterval,
		true);
}

void UAutoAttack::Update(float DeltaTime)
{
	for (auto& [Type, ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }
		ConeSlashParams->Update(DeltaTime);
	}

}

void UAutoAttack::SetVisibility(bool bVisibility)
{
	for (auto& [Type, ConeSlashParams] : AutoAttackParamsMap)
	{
		if (!ConeSlashParams) { continue; }
		ConeSlashParams->SetVisibility(bVisibility);
	}
}

void UAutoAttack::StartAutoAttack()
{
	if (!GetOwnerActor()) { return; }

	if(AutoAttackParamsMap.Contains(EAutoAttackType::Front)
		&& AutoAttackParamsMap[EAutoAttackType::Front])
	{
		AutoAttackParamsMap[EAutoAttackType::Front]->Execute();
	}

	// 前方扇状自動攻撃からの周囲攻撃遅延タイマーをセット
	GetWorld()->GetTimerManager().SetTimer(
		FrontToRingDelayTimerHandle,
		this,
		&UAutoAttack::StartAutoRingAttack,
		FrontToRingDelay,
		false);
}

void UAutoAttack::StartAutoRingAttack()
{
	if (AutoAttackParamsMap.Contains(EAutoAttackType::Ring)
		&& AutoAttackParamsMap[EAutoAttackType::Ring])
	{
		AutoAttackParamsMap[EAutoAttackType::Ring]->Execute();
	}
}