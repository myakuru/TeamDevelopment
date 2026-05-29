#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>


#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>
#include <ProjectNull/System/Material/PlayerMaterialCollectionUpdater/PlayerMaterialCollectionUpdater.h>
#include <ProjectNull/Actor/Effect/ModelAfterimageTrailEffect/ModelAfterimageTrailEffect.h>


APlayerBase::APlayerBase():
		SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		GearComponent(nullptr),
		TargetSearchComponent(nullptr),
		AutoAttack(nullptr),
		MaterialCollectionUpdater(nullptr),
		SuperGameInstance(nullptr)
{
	// ================================================================
	// プレイヤーの初期化
	// ================================================================
	PrimaryActorTick.bCanEverTick	= true;
	bUseControllerRotationYaw		= false;

	// ================================================================
	// スプリングアームの初期化
	// ================================================================
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>("Spring Arm");
	if (!SpringArmComponent) { return; }
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;

	// ================================================================
	// カメラコンポーネントの初期化
	// ================================================================
	CameraComponent = CreateDefaultSubobject<UCameraComponent>("Camera");
	if (!CameraComponent) { return; }
	CameraComponent->SetupAttachment(SpringArmComponent);
	CameraComponent->bUsePawnControlRotation = false;

	// ================================================================
	// ギアコンポーネントの初期化
	// ================================================================
	GearComponent = CreateDefaultSubobject<UPlayerGearComponent>("Gear");

	// ================================================================
	// 対象検索コンポーネントの初期化
	// ================================================================
	TargetSearchComponent = CreateDefaultSubobject<UTargetSearchComponent>("TargetSearch");

	// Material Parameter Collectionの更新処理クラスの生成
	MaterialCollectionUpdater = NewObject<UPlayerMaterialCollectionUpdater>();
}

void APlayerBase::BeginPlay()
{
	ACombatCharacterBase::BeginPlay();

	// ================================================================
	// ゲーム全体で共有されるデータや機能を管理するクラスの初期化
	// ================================================================
	SuperGameInstance = GetWorld()->GetGameInstance<USuperGameInstance>();

	// ================================================================
	// 自動攻撃の初期化
	// ================================================================
	if (AutoAttack) { AutoAttack->Initialize(this); }

	// ================================================================
	// Material Parameter Collectionの更新処理クラスの初期化
	// ================================================================
	if (MaterialCollectionUpdater) { MaterialCollectionUpdater->Initialize(this); }

}

void APlayerBase::Tick(float DeltaTime)
{
	auto* EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!EnemyManager) { return; }

	ACombatCharacterBase::Tick(DeltaTime);

	// 自動攻撃の更新
	if (AutoAttack) { AutoAttack->Update(DeltaTime,nullptr, EnemyManager); }

	// Material Parameter Collectionの更新処理クラスの更新
	if (MaterialCollectionUpdater) { MaterialCollectionUpdater->Update(DeltaTime); }
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerBase::Move(const FVector2d& InputVector)
{
	if (!CanMove()) { return; }

	const FRotator	YawRotation = { 0.f, GetControlRotation().Yaw, 0.f };
	const FVector	Forward		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector	Right		= FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, InputVector.Y);
	AddMovementInput(Right, InputVector.X);
}

void APlayerBase::ChangeGear()
{
	if (!GearComponent) { return; }
	GearComponent->ChangeGear();
}

int32 APlayerBase::GetCurrentGearLevel() const
{
	if (!GearComponent) { return 0; }
	return GearComponent->GetCurrentGearLevel();
}

UPlayerAnimInstance* APlayerBase::GetPlayerAnimInstance() const
{
	if (!GetMesh() || !GetMesh()->GetAnimInstance()) { return nullptr; }
	return Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
}

FPoseSnapshot& APlayerBase::GetPlayerPoseSnapshot()
{
	FPoseSnapshot PoseSnapshot;
	if (!GetMesh() || !GetMesh()->GetAnimInstance()
		|| !Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance())) { return PoseSnapshot; }

	auto* PlayerAnimInstance = Cast<UPlayerAnimInstance>(GetMesh()->GetAnimInstance());
	return PlayerAnimInstance->GetPlayerPoseSnapshot();
}

bool APlayerBase::CanMove()
{
	if(GearComponent && GearComponent->IsMovementBlockedByGear())
	{
		return false;
	}

	return true;
}

