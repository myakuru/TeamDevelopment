
#include "FloatingWeaponEffect.h"

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

#include <ProjectNull/System/Combat/Attack/RingPulseSlashAttack/RingPulseSlashAttack.h>


UFloatingWeaponEffect::UFloatingWeaponEffect():
	OwnerAttack(nullptr),
	EffectSystem(nullptr),
	EffectComponent(nullptr),
	Transform(FTransform()),
	RadiusOffset(200.0f),
	RotatorOffset(FRotator())
{

}

void UFloatingWeaponEffect::Start(USceneComponent* RootComponent)
{
	if (!CanSpawn()) { return; }
	//　エフェクトの再生開始
	EffectComponent = UNiagaraFunctionLibrary::SpawnSystemAttached(
		EffectSystem,
		RootComponent,
		NAME_None,
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::KeepRelativeOffset,
		true);
}

void UFloatingWeaponEffect::Update(AActor* OwnerActor)
{
	if (!OwnerActor || !OwnerAttack) { return; }

	//　攻撃が消えたらエフェクト消す
	if (OwnerAttack->CanDeactivate())
	{
		Deactivate();
	}
	
	//　PrevActiveフラグの更新
	OwnerAttack->UpdatePrevActiveFlg();

	//　プレイヤーの座標
	const FVector playerLocation		= OwnerActor->GetActorLocation();
	//　プレイヤーが向いてる方向
	const FVector playerForwardVector	= OwnerActor->GetActorForwardVector();
	//　攻撃方向からのオフセット位置
	const FVector offsetLocation		= OwnerAttack->CalcAttackDir(playerForwardVector) * RadiusOffset;
	//　浮遊武器の最終位置
	const FVector resultLocation		= playerLocation + offsetLocation;
	
	Transform.SetLocation(resultLocation);

	RotatorOffset.Yaw = OwnerActor->GetActorRotation().Yaw + OwnerAttack->CurrentAngle;
	Transform.SetRotation(RotatorOffset.Quaternion());

	UpdateTransform();
}

void UFloatingWeaponEffect::UpdateTransform()
{
	if (!EffectComponent) { return; }
	EffectComponent->SetWorldTransform(Transform);
}

void UFloatingWeaponEffect::Deactivate()
{
	if (!EffectComponent) { return; }
	EffectComponent->DestroyComponent();
	EffectComponent = nullptr;
}
