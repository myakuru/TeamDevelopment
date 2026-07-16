#include "PlayerBase.h"

#include "CineCameraComponent.h"

#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "Camera/CameraComponent.h"
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/Component/TargetSearchComponent/TargetSearchComponent.h>
#include <ProjectNull/Component/HitStopComponent/HitStopComponent.h>
#include <ProjectNull/Component/GroundAlignmentComponent/GroundAlignmentComponent.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
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
		SuperGameInstance(nullptr),
		NormalStateCameraLagSpeed(0.f),
		TargetCameraLagSpeed(0.f),
		CameraLagInterpSpeed(0.f)
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
	if (!SuperGameInstance) { return; }

	const auto PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
	if (!PlayerRuntimeData) { return; }
	PlayerRuntimeData->SetOwner(this);
	PlayerRuntimeData->UpdateStatus();
	
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

	if (!SpringArmComponent) { return; }

	NormalStateCameraLagSpeed = SpringArmComponent->CameraLagSpeed;
	ResetTargetCameraLagSpeed();
}

void APlayerBase::Tick(float DeltaTime)
{
	ACombatCharacterBase::Tick(DeltaTime);

	// 自動攻撃の更新
	if (AutoAttack) { AutoAttack->Update(DeltaTime); }

	// Material Parameter Collectionの更新処理クラスの更新
	if (MaterialCollectionUpdater) { MaterialCollectionUpdater->Update(DeltaTime); }

	UpdateModelAfterimageTrailEffect(DeltaTime);

	UpdateCameraData(DeltaTime);
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
	
}

float APlayerBase::GetFinalAttackPower() const
{
	if (!SuperGameInstance) { return 1.f; }

	const auto PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
	if (PlayerRuntimeData)	{ return 1.f; }

	return PlayerRuntimeData->GetCharacterAttackPower();
}

void APlayerBase::ApplyDamaged(float InDamage)
{
	if (!SuperGameInstance) { return; }

	const auto PlayerRuntimeData = SuperGameInstance->GetPlayerRuntimeData();
	if (PlayerRuntimeData)	{ return; }

	PlayerRuntimeData->SubtractHealth(InDamage);
	//UE_LOG(LogTemp, Warning, TEXT("PlayerHP : %f"), PlayerRuntimeData->GetHealth());
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

void APlayerBase::ResetTargetCameraLagSpeed()
{
	TargetCameraLagSpeed = NormalStateCameraLagSpeed;
}

int32 APlayerBase::GetCurrentGearLevel() const
{
	if (!GearComponent) { return 0; }
	return GearComponent->GetCurrentGearLevel();
}

bool APlayerBase::GetCurrentFloorNormal(FVector& OutCurrentFloorNormal)
{
	const auto CharacterMovementComp = GetCharacterMovement();
	if (!CharacterMovementComp) { return false; }
	OutCurrentFloorNormal = CharacterMovementComp->CurrentFloor.HitResult.ImpactNormal;
	return true;
}

USceneComponent* APlayerBase::GetGroundAlignmentRootComponent() const
{
	if (!GroundAlignmentComponent) { return nullptr; }
	const auto RootComp = GroundAlignmentComponent->GetRootComponent();
	
	return RootComp;
}

UPlayerAnimInstance* APlayerBase::GetPlayerAnimInstance() const
{
	const auto PlayerMesh = GetMesh();
	if (!PlayerMesh)	{ return nullptr; }

	const auto AnimInstance = PlayerMesh->GetAnimInstance();
	if (!AnimInstance)	{ return nullptr; }

	const auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
	return PlayerAnimInstance;
}

FPoseSnapshot& APlayerBase::GetPlayerPoseSnapshot()
{
	FPoseSnapshot PoseSnapshot;

	const auto PlayerMesh = GetMesh();
	if (!PlayerMesh)			{ return PoseSnapshot; }

	const auto AnimInstance = PlayerMesh->GetAnimInstance();
	if (!AnimInstance)			{ return PoseSnapshot; }

	const auto PlayerAnimInstance = Cast<UPlayerAnimInstance>(AnimInstance);
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

void APlayerBase::UpdateCameraData(float DeltaTime)
{
	if (!SpringArmComponent) { return; }

	SpringArmComponent->CameraLagSpeed = FMath::FInterpTo(
		SpringArmComponent->CameraLagSpeed,
		TargetCameraLagSpeed,
		DeltaTime,
		CameraLagInterpSpeed
		);
}

void APlayerBase::UpdateModelAfterimageTrailEffect(float DeltaTime)
{
	const auto PlayerMesh = GetMesh();
	if (!PlayerMesh)					{ return; }
	
	const auto PlayerAnimInstance = GetPlayerAnimInstance();
	if (!PlayerAnimInstance)			{ return; }

	if (!ModelAfterimageTrailEffect)	{ return; }

	ModelAfterimageTrailEffect->Update(
		DeltaTime,
		GetActorTransform(),
		PlayerMesh->GetSkeletalMeshAsset(),
		PlayerAnimInstance->GetPlayerPoseSnapshot()
	);
}


