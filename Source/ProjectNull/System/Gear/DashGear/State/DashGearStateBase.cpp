
#include "DashGearStateBase.h"


#include <ProjectNull/Actor/Effect/EffectBase.h>
#include <ProjectNull/Actor/Effect/ModelAfterimageTrailEffect/ModelAfterimageTrailEffect.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Gear/DashGear/DashGear.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>

#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

#include <GameFramework/CharacterMovementComponent.h>

#include <ProjectNull/Utility/GroundUtility/GroundUtility.h>

#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"

UDashGearStateBase::UDashGearStateBase():
	DashAnimMontage(nullptr),
	DashEffect(nullptr),
	DashGear(nullptr),
	DashDir(FVector::ZeroVector),
	StartQuat(FQuat::Identity),
	DashSpeed(2000.0f),
	DashEffectDuration(0.3f),
	MontageBlendOutTime(0.2f),
	DashSphereRadius(200.f),
	MaxDashSlopeAngle(30.f)
{
}

void UDashGearStateBase::Initialize(
	APlayerBase* InPlayer, 
	UPlayerGearComponent* InGearComponent,
	UGearBase* InOwner)
{
	UGearStateBase::Initialize(
		InPlayer,
		InGearComponent,
		InOwner
	);

	DashGear = Cast<UDashGear>(Owner);
}

void UDashGearStateBase::Execute(int32 CurrentGearLevel)
{
	UGearStateBase::Execute(CurrentGearLevel);

	ExecuteDash();

}

void UDashGearStateBase::Update(float DeltaTime)
{
	UGearStateBase::Update(DeltaTime);

	Dash();
}

void UDashGearStateBase::End()
{
	UGearStateBase::End();
	EndDash();
}

void UDashGearStateBase::ExecuteDash()
{
	if (!Player || 
		!DashGear) { return; }

	auto GroundAlignmentComp = Player->GetGroundAlignmentComponent();
	if (!GroundAlignmentComp) { return; }

	auto RootComp = GroundAlignmentComp->GetRootComponent();
	if (!RootComp) { return; }

	auto CharacterMovementComp = Player->GetCharacterMovement();
	if (!CharacterMovementComp) { return; }

	FVector CurrentFloorNormal = FVector::ZeroVector;
	float SlopeAngle = 0.f;
	if (Player->GetCurrentFloorNormal(CurrentFloorNormal))
	{
		float Dot = FVector::DotProduct(CurrentFloorNormal.GetSafeNormal(), FVector::UpVector);
		Dot = FMath::Clamp(Dot, -1.0f, 1.0f);
		SlopeAngle = FMath::RadiansToDegrees(FMath::Acos(Dot));
	}
	//UE_LOG(LogTemp, Display, TEXT("SlopeAngle X%.2f"), SlopeAngle);

	const bool IsOnGround = CharacterMovementComp->IsMovingOnGround();

	if (MaxDashSlopeAngle <= SlopeAngle && IsOnGround)
	{
		DashGear->ForceStop();
		return;
	}

	DashGear->SetSphereRadius(DashSphereRadius);

	InitializeStartDashData(RootComp);
	PlayDashNiagaraEffect(RootComp);
	PlayDashAnimation();
	SetSphereCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	SetEnableSpawnAfterimage(true);
}

void UDashGearStateBase::EndDash()
{
	BlendOutDashAnimation();
	DeactivateNiagaraEffect();
	SetSphereCollisionEnabled(ECollisionEnabled::NoCollision);
	SetEnableSpawnAfterimage(false);
}

void UDashGearStateBase::InitializeStartDashData(USceneComponent* InGroundAlignmentComp)
{
	if (!InGroundAlignmentComp) { return; }
	DashDir		= InGroundAlignmentComp->GetForwardVector();
	StartQuat	= InGroundAlignmentComp->GetComponentQuat();
}

void UDashGearStateBase::Dash()
{
	if (!Player ||
		!Owner)				{ return; }

	Player->LaunchCharacter(DashDir * DashSpeed, true, true);
	//UE_LOG(LogTemp, Display, TEXT("DashDir X%.2f Y%.2f Z%.2f"), DashDir.X,DashDir.Y,DashDir.Z);

	if (Owner) {
		Owner->SetBlocksMovement(true);
	}
	
}

void UDashGearStateBase::PlayDashNiagaraEffect(USceneComponent* InGroundAlignmentComp)
{
	if (!DashEffect) { return; }
	DashEffect->Start(InGroundAlignmentComp);
	// 位置だけ親に追従
	DashEffect->SetAbsolute(false, true, true);

	auto EffectComp = DashEffect->GetEffectComponent();
	if (!EffectComp) { return; }

	EffectComp->SetWorldRotation(StartQuat);
}

void UDashGearStateBase::DeactivateNiagaraEffect()
{
	if (!DashEffect) { return; }
	DashEffect->DeactivateEffect();
}

void UDashGearStateBase::PlayDashAnimation()
{
	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Play(DashAnimMontage);
}

void UDashGearStateBase::BlendOutDashAnimation()
{
	if (!Player) { return; }

	auto PlayerAnimInstance = Player->GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	PlayerAnimInstance->Montage_Stop(MontageBlendOutTime);
}

void UDashGearStateBase::SetSphereCollisionEnabled(const ECollisionEnabled::Type InEnabled)
{
	if (!DashGear) { return; }
	DashGear->SetSphereCollisionEnabled(InEnabled);
}

void UDashGearStateBase::SetEnableSpawnAfterimage(bool bInEnableSpawn)
{
	if (!Player) { return; }
	auto ModelAfterimageTrailEffect = Player->GetModelAfterimageTrailEffect();

	if (!ModelAfterimageTrailEffect) { return; }
	ModelAfterimageTrailEffect->SetEnableSpawn(bInEnableSpawn);
}
