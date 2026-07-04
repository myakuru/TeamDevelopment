#include "PlayerGearComponent.h"

#include "Components/SphereComponent.h"

#include <ProjectNull/Component/HitStopComponent/HitStopComponent.h>

#include <ProjectNull/Actor/CollisionActor/SphereCollision/SphereCollision.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyBase.h>

#include <ProjectNull/Actor/Effect/EffectBase.h>

#include <ProjectNull/GameInstance/SuperGameInstance.h>

#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>


UPlayerGearComponent::UPlayerGearComponent():
		OwnerPlayer(nullptr),
		PlayerRuntimeData(nullptr),
		PlayerParameterData(nullptr),
		PlayerGears(TArray<TObjectPtr<UGearBase>>()),
		SphereCollision(nullptr),
		SphereCollisionClass(nullptr),
		CurrentGearLevel(1),
		HitStopDuration(0.f),
		HitStopTimeDilation(0.f),
		InvincibilityTimerHandle(FTimerHandle())
{
	PrimaryComponentTick.bCanEverTick = true;

}

void UPlayerGearComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerBase>(GetOwner());

	auto SuperGameInstance	= GetWorld()->GetGameInstance<USuperGameInstance>();
	if (!SuperGameInstance)		{ return; }

	PlayerParameterData		= SuperGameInstance->GetPlayerParameterData();
	if (!PlayerParameterData)	{ return; }

	PlayerRuntimeData		= SuperGameInstance->GetPlayerRuntimeData();
	if (!PlayerRuntimeData)		{ return; }

	InitializeSphereCollision();

	for (int32 Index = 0; Index < PlayerGears.Num(); ++Index)
	{
		if (!PlayerGears[Index]) { continue; }
		PlayerGears[Index]->Initialize(OwnerPlayer, this);
		PlayerGears[Index]->SetGearIndex(Index);
		PlayerParameterData->ResetSkillCooldown(Index);
	}

}

void UPlayerGearComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UpdateGearWidget(DeltaTime);

	for(auto& Gear : PlayerGears) {
		if (!Gear) { continue; }
		Gear->Update(DeltaTime);
	}

	UpdateCollisionByInvincibility();

	/*if (!PlayerRuntimeData) { return; }
	UE_LOG(LogTemp, Warning, TEXT("hi IsInvincible %d"), PlayerRuntimeData->IsInvincible());*/

}

bool UPlayerGearComponent::IsMovementBlockedByGear() const
{
	for (auto& Gear : PlayerGears) {
		if (!Gear) { continue; }
		if (Gear->BlocksMovement()) {
			return true;
		}
	}
	return false;
}

void UPlayerGearComponent::ExecuteGear(int32 GearIndex)
{
	if (PlayerGears.IsValidIndex(GearIndex)) {
		if (PlayerGears[GearIndex]) {
			PlayerGears[GearIndex]->Execute(CurrentGearLevel);
		}
	}
}

void UPlayerGearComponent::ChangeGear()
{
	if (!OwnerPlayer ||
		!PlayerRuntimeData ||
		!PlayerParameterData)	{ return; }

	if (!CanChangeGear())		{ return; }

	PlayerRuntimeData->ResetDataOnGearChange(CurrentGearLevel);
	
	// プレイヤーのギアパラメータデータ取得
	const auto GearParameterData = PlayerParameterData->GetGearData();

	PlayerRuntimeData->CalculateInvincibilityTime(GearParameterData);
	CurrentGearLevel = (CurrentGearLevel % 4 + 1);
	//UE_LOG(LogTemp, Warning, TEXT("hi level %d"), CurrentGearLevel);

	OnInvincibilityStart();

}

void UPlayerGearComponent::OnGearBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult)
{
	if (!OwnerPlayer)	{ return; }
	const FVector PlayerLocation = OwnerPlayer->GetActorLocation();

	auto Interface = Cast<ICharacterInterface>(OtherActor);
	if (!Interface)		{ return; }
	Interface->ApplyDamaged(1.f);
	Interface->ApplyKnockBack(PlayerLocation);
	
	auto HitStopComp = OwnerPlayer->GetHitStopComponent();
	if (!HitStopComp)	{ return; }

	HitStopComp->StartHitStop(
		HitStopDuration,
		HitStopTimeDilation
	);

}

void UPlayerGearComponent::InitializeSphereCollision()
{
	SphereCollision = GetWorld()->SpawnActor<ASphereCollision>
		(SphereCollisionClass);

	if (!SphereCollision) { return; }

	// 攻撃用球アクターに親をアタッチ
	SphereCollision->AttachToActor(
		OwnerPlayer,
		FAttachmentTransformRules::KeepRelativeTransform
	);

	auto SphereComponent = SphereCollision->GetSphereComponent();
	if (!SphereComponent) { return; }

	// オーバーラップ時、オーバーラップ抜け時の関数をセット
	SphereComponent->OnComponentBeginOverlap.AddDynamic(
		this,
		&UPlayerGearComponent::OnGearBeginOverlap
	);

	SphereCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void UPlayerGearComponent::UpdateSkillCooldown(
	int32 Index,
	UGearBase* Gear)
{
	if (!PlayerParameterData ||
		!Gear) { return; }

	const FTimerHandle	CoolTimerHandle		= Gear->GetCoolTimerHandle();
	FTimerManager&		TimerManager		= GetWorld()->GetTimerManager();
	const bool			IsCooldownActive	= TimerManager.IsTimerActive(CoolTimerHandle);
	const float			RemainTime			= IsCooldownActive ? TimerManager.GetTimerRemaining(CoolTimerHandle) : 0.f;

	PlayerParameterData->UpdateSkillCooldown(
		Index, 
		RemainTime,
		Gear->GetGearCoolTime(Index));
}

void UPlayerGearComponent::SetIsInvincible(bool bInIsInvincible)
{
	if (!PlayerRuntimeData ||
		!SphereCollision) { return; }

	PlayerRuntimeData->SetIsInvincible(bInIsInvincible);
	
	ECollisionEnabled::Type CollisionType
		= bInIsInvincible ? ECollisionEnabled::QueryOnly : ECollisionEnabled::NoCollision;


	if (bInIsInvincible)
	{
		StartInvincibleEffect();
	}
	else {
		DeactivateEffect();
	}

	SphereCollision->SetCollisionEnabled(CollisionType);
}

bool UPlayerGearComponent::CanChangeGear() const
{
	if (!PlayerRuntimeData) { return false; }
	return PlayerRuntimeData->CanChangeGear(CurrentGearLevel);
}

void UPlayerGearComponent::OnInvincibilityStart()
{
	if (!PlayerRuntimeData) { return; }

	const auto& GearRuntimeData = PlayerRuntimeData->GetGearData();
	
	SetIsInvincible(true);

	GetWorld()->GetTimerManager().SetTimer(
		InvincibilityTimerHandle,
		this,
		&UPlayerGearComponent::OnInvincibilityEnd,
		GearRuntimeData.GearChangeInvincibilityTime,
		false);
}

void UPlayerGearComponent::OnInvincibilityEnd()
{
	if (!PlayerRuntimeData) { return; }

	if (CurrentGearLevel == kMaxGearLevel) {
		CurrentGearLevel = 1;
		PlayerRuntimeData->LevelUp();
	}

	SetIsInvincible(false);
}

void UPlayerGearComponent::UpdateCollisionByInvincibility()
{
	if (!OwnerPlayer ||
		!PlayerRuntimeData)					{ return; }

	if (!PlayerRuntimeData->IsInvincible()) { return; }
	//UE_LOG(LogTemp, Warning, TEXT("hi addmove"));
	
	const FRotator	YawRotation	= { 0.f, OwnerPlayer->GetControlRotation().Yaw, 0.f };
	const FVector	Forward		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

	OwnerPlayer->AddMovementInput(Forward);
}

void UPlayerGearComponent::UpdateGearWidget(float DeltaTime)
{
	for (int32 Index = 0; Index < PlayerGears.Num(); ++Index)
	{
		UpdateSkillCooldown(Index, PlayerGears[Index]);
	}
}

void UPlayerGearComponent::StartInvincibleEffect()
{
	if (!InvincibleEffect ||
		!OwnerPlayer)		{ return; }

	const auto RootComp = OwnerPlayer->GetRootComponent();
	if (!RootComp)			{ return; }

	InvincibleEffect->Start(RootComp);
}

void UPlayerGearComponent::DeactivateEffect()
{
	if (!InvincibleEffect) { return; }
	InvincibleEffect->DeactivateEffect();
}

