#include "PlayerBase.h"

#include "CineCameraComponent.h"

#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

#include "Camera/CameraComponent.h"
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>
#include <ProjectNull/Component/HitStopComponent/HitStopComponent.h>
#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>

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
#include <ProjectNull/Component/PlayerCutsceneComponent/PlayerCutsceneComponent.h>
#include <ProjectNull/Core/NullGameplayTags.h>


APlayerBase::APlayerBase():
		SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		CineCameraComponent(nullptr),
		GearComponent(nullptr),
		TargetSearchComponent(nullptr),
		HitStopComponent(nullptr),
		GroundAlignmentComponent(nullptr),
		AutoAttack(nullptr),
		MaterialCollectionUpdater(nullptr),
		CutsceneComponent(nullptr),
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
	// シネマティックカメラコンポーネントの初期化
	// ================================================================
	CineCameraComponent = CreateDefaultSubobject<UCineCameraComponent>("CineCamera");
	CineCameraComponent->SetupAttachment(SpringArmComponent);
	CineCameraComponent->Deactivate();

	// ================================================================
	// カットシーン再生用コンポーネントの初期化
	// ================================================================
	CutsceneComponent = CreateDefaultSubobject<UPlayerCutsceneComponent>("Cutscene");

	// ================================================================
	// ギアコンポーネントの初期化
	// ================================================================
	GearComponent = CreateDefaultSubobject<UPlayerGearComponent>("Gear");

	// ================================================================
	// 対象検索コンポーネントの初期化
	// ================================================================
	TargetSearchComponent = CreateDefaultSubobject<UTargetSearchComponent>("TargetSearch");

	// ================================================================
	// ヒットストップコンポーネントの初期化
	// ================================================================
	HitStopComponent = CreateDefaultSubobject<UHitStopComponent>("HitStop");

	// ================================================================
	// 地面の法線に合わせてRootComponentの姿勢を更新するコンポーネント初期化
	// ================================================================
	GroundAlignmentComponent = CreateDefaultSubobject<UGroundAlignmentComponent>("GroundAlignment");

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

	GetWorld()->GetFirstPlayerController()->InputComponent->BindKey(
		EKeys::K, IE_Pressed, this, &APlayerBase::StartCutscene);
}

void APlayerBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);

	// 自動攻撃の更新
	if (AutoAttack) { AutoAttack->Update(DeltaTime); }

	// Material Parameter Collectionの更新処理クラスの更新
	if (MaterialCollectionUpdater) { MaterialCollectionUpdater->Update(DeltaTime); }

	UpdateModelAfterimageTrailEffect(DeltaTime);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerBase::ApplyDamaged(float InDamage)
{
	auto GameInstance = Cast<USuperGameInstance>(GetWorld()->GetGameInstance());

	if (GameInstance&& GameInstance->GetPlayerRuntimeData())
	{
		auto PlayerRuntimeData = GameInstance->GetPlayerRuntimeData();

		PlayerRuntimeData->SubtractHealth(InDamage);

		//UE_LOG(LogTemp, Warning, TEXT("PlayerHP : %f"), PlayerRuntimeData->GetHealth());
	}
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

void APlayerBase::StartCutscene()
{
	if (!CutsceneComponent) { return; }
	CutsceneComponent->PlayCutScene(NullGameplayTags::Cutscene_Intro);
}

int32 APlayerBase::GetCurrentGearLevel() const
{
	if (!GearComponent) { return 0; }
	return GearComponent->GetCurrentGearLevel();
}

bool APlayerBase::GetCurrentFloorNormal(FVector& OutCurrentFloorNormal)
{
	auto CharacterMovementComp = GetCharacterMovement();
	if (!CharacterMovementComp) { return false; }
	OutCurrentFloorNormal = CharacterMovementComp->CurrentFloor.HitResult.ImpactNormal;
	return true;
}

UPlayerAnimInstance* APlayerBase::GetPlayerAnimInstance() const
{
	if (!GetMesh())		{ return nullptr; }

	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)	{ return nullptr; }

	auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
	return PlayerAnimInstance;
}

FPoseSnapshot& APlayerBase::GetPlayerPoseSnapshot()
{
	FPoseSnapshot PoseSnapshot;

	if (!GetMesh())				{ return PoseSnapshot; }

	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance)			{ return PoseSnapshot; }

	auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
	if (!PlayerAnimInstance)	{ return PoseSnapshot; }

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

void APlayerBase::UpdateModelAfterimageTrailEffect(float DeltaTime)
{
	if (!ModelAfterimageTrailEffect || 
		!GetMesh()) { return; }

	const auto PlayerAnimInstance = GetPlayerAnimInstance();
	if (!PlayerAnimInstance) { return; }

	ModelAfterimageTrailEffect->Update(
		DeltaTime,
		GetActorTransform(),
		GetMesh()->GetSkeletalMeshAsset(),
		PlayerAnimInstance->GetPlayerPoseSnapshot());
}


