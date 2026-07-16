
#include "FloatingWeaponAttack.h"


#include <ProjectNull/Actor/Effect/SlashEffect/SlashEffectBase.h>
#include <ProjectNull/Actor/HitActor/AutoAttackHitActor/AutoAttackHitActor.h>
#include <ProjectNull/Actor/Effect/FloatingWeaponEffect/FloatingWeaponEffect.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/Utility/GroundUtility/GroundUtility.h>

UFloatingWeaponAttack::UFloatingWeaponAttack():
	AutoAttack(nullptr),
	FloatingWeaponEffect(nullptr),
	SlashEffectArray(TArray<TObjectPtr<USlashEffectBase>>()),
	AutoAttackHitActor(nullptr),
	AutoAttackHitActorClass(nullptr),
	StandTimeRatio(0.f),
	Duration(1.0f),
	ElapsedTime(0.0f),
	RotationSpeed(1000.0f),
	bPrevActive(false),
	CurrentAngle(0.0f),
	KnockbackPower(2.0f),
	StartAngle(0.0f),
	RotationInterpSpeed(0.0f),
	bEffectVisibility(true)
{

}

void UFloatingWeaponAttack::Initialize(const TObjectPtr<AActor>& Owner)
{
	Super::Initialize(Owner);

	// 自身のRootは持ち主のクラスのRootにアタッチ済み

	// 自動攻撃の当たり判定アクターを出現
	AutoAttackHitActor = GetWorld()->SpawnActor<AAutoAttackHitActor>
		(AutoAttackHitActorClass);

	// 自身のRoot取得
	const auto RootComp = GetRootComponent();
	if (!RootComp)				{ return; }

	if (!AutoAttackHitActor)	{ return; }

	// 自動攻撃の当たり判定アクターを
	AutoAttackHitActor->AttachToComponent(
		RootComp,
		FAttachmentTransformRules::KeepRelativeTransform);

	if (!FloatingWeaponEffect)	{ return; }
	
	FloatingWeaponEffect->SetOwnerAttack(this);
	FloatingWeaponEffect->SetOwnerActor(Owner);
	FloatingWeaponEffect->Initialize();
	FloatingWeaponEffect->Start(RootComp);
	
}

void UFloatingWeaponAttack::Update(float DeltaTime)
{
	AlignFloor(DeltaTime);
	
	UpdateRotation(DeltaTime);
	
	if (FloatingWeaponEffect) {
		FloatingWeaponEffect->Update(DeltaTime);
	}
}

void UFloatingWeaponAttack::Execute(const FVector& InTargetLocation)
{
	const auto Owner = GetOwnerActor();
	if (!Owner)					{ return; }

	SetIsActive(true);
	CurrentAngle	= StartAngle;
	ElapsedTime		= 0.0f;

	if (!AutoAttackHitActor)	{ return; }
	AutoAttackHitActor->SetHitEnabled(true);

	// 自身のRoot取得
	const auto RootComp = GetRootComponent();
	if (!RootComp)				{ return; }

	
	if (!bEffectVisibility) { return; }
	
	for (auto& SlashEffect : SlashEffectArray)
	{
		if (!SlashEffect) { continue; }
		SlashEffect->StartOnce(RootComp);
	}
}

void UFloatingWeaponAttack::SetVisibility(bool bVisibility)
{
	bEffectVisibility = bVisibility;
	
	if (FloatingWeaponEffect)
	{
		FloatingWeaponEffect->SetVisibility(bVisibility);
	}
	
	for (auto& SlashEffect : SlashEffectArray)
	{
		if (!SlashEffect) { continue; }
		SlashEffect->SetVisibility(bVisibility);
	}
}

bool UFloatingWeaponAttack::IsAttackStateStep()
{
	if (!AutoAttack) { return false; }
	return ElapsedTime < Duration;
}

float UFloatingWeaponAttack::TotalTransitionStateTime()
{
	float ResultTime = 0.f;
	if (!AutoAttack) { return ResultTime; }
	const float Interval = AutoAttack->GetAutoAttackInterval();
	const float TotalTransitionTimeRatio = 1.f - StandTimeRatio;
	ResultTime = (Interval - Duration) * TotalTransitionTimeRatio;
	return ResultTime;
}

float UFloatingWeaponAttack::StandStateTime()
{
	float ResultTime = 0.f;
	if (!AutoAttack) { return ResultTime; }
	const float Interval = AutoAttack->GetAutoAttackInterval();
	ResultTime = (Interval - Duration) * StandTimeRatio;
	return ResultTime;
}

FVector UFloatingWeaponAttack::CalcAttackDir(const FVector& ForwardVector, float Angle) const
{
	return ForwardVector.RotateAngleAxis(Angle, FVector::UpVector);
}

bool UFloatingWeaponAttack::CanDeactivate()
{
	const bool bCurrentActive = IsActive();
	bool canDeactivate = (bCurrentActive != bPrevActive) && !bCurrentActive;
	bPrevActive = bCurrentActive;
	return canDeactivate;
}

bool UFloatingWeaponAttack::IsActiveFirstFrame()
{
	const bool bCurrentActive = IsActive();
	bool canDeactivate = (bCurrentActive != bPrevActive) && bCurrentActive;
	bPrevActive = bCurrentActive;
	bPrevActive = bCurrentActive;
	return canDeactivate;
}

void UFloatingWeaponAttack::UpdatePrevActiveFlg()
{
	bPrevActive = IsActive();
}

void UFloatingWeaponAttack::UpdateRotation(float DeltaTime)
{
	if (!IsActive()) { return; }

	ElapsedTime += DeltaTime;

	// 回転処理
	CurrentAngle += RotationSpeed * DeltaTime;

	if (!AutoAttackHitActor) { return; }

	AutoAttackHitActor->SetActorRelativeRotation(
		FRotator(0.f, CurrentAngle, 0.f));

	// 終了判定
	if (ElapsedTime >= Duration) {
		SetIsActive(false);
		CurrentAngle = StartAngle;
		AutoAttackHitActor->SetHitEnabled(false);
	}
}

void UFloatingWeaponAttack::AlignFloor(float DeltaTime)
{
	auto RootComp	= GetRootComponent();
	if (!RootComp)										{ return; }

	auto Player		= Cast<APlayerBase>(GetOwnerActor());
	if (!Player)										{ return; }

	FVector FloorNormal = FVector::ZeroVector;
	if (!Player->GetCurrentFloorNormal(FloorNormal))	{ return; }

	if (FloorNormal.IsNearlyZero()) {
		FloorNormal = FVector::UpVector;
	}

	const FQuat TargetQuat	= UGroundUtility::MakeRotationFromGroundNormal(
		Player->GetActorTransform(),
		FloorNormal);

	const FQuat NewQuat		= FQuat::Slerp(
		RootComp->GetComponentQuat(),
		TargetQuat,
		DeltaTime * RotationInterpSpeed);

	RootComp->SetWorldRotation(NewQuat);
}