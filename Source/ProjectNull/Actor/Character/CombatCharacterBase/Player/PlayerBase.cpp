#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include "CineCameraComponent.h"

#include <GameFramework/SpringArmComponent.h>
#include <GameFramework/CharacterMovementComponent.h>

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "Components/SphereComponent.h"

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
		CineCameraComponent(nullptr),
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
	// シネマティックカメラコンポーネントの初期化
	// ================================================================
	CineCameraComponent = CreateDefaultSubobject<UCineCameraComponent>("CineCamera");
	CineCameraComponent->SetupAttachment(SpringArmComponent);
	CineCameraComponent->Deactivate();

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

	/*GetWorld()->GetTimerManager().SetTimer(
		AlignFloorTimerHandle,
		this,
		&APlayerBase::AlignFloor,
		0.1f,
		true);*/
	CurrentGroundTraceLength = GroundTraceLength;
}

void APlayerBase::Tick(float DeltaTime)
{
	auto* EnemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!EnemyManager) { return; }

	ACombatCharacterBase::Tick(DeltaTime);

	// 自動攻撃の更新
	if (AutoAttack) { AutoAttack->Update(DeltaTime); }

	// Material Parameter Collectionの更新処理クラスの更新
	if (MaterialCollectionUpdater) { MaterialCollectionUpdater->Update(DeltaTime); }


	//AlignFloor();
	
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

//FVector& APlayerBase::GetCurrentFloorNormal() const
//{
//	auto CharacterMovementComp = GetCharacterMovement();
//	FVector OutVector = FVector::ZeroVector;
//	if (!CharacterMovementComp) { return OutVector; }
//	OutVector = CharacterMovementComp->CurrentFloor.HitResult.ImpactNormal;
//	return OutVector;
//}

UPlayerAnimInstance* APlayerBase::GetPlayerAnimInstance() const
{
	if (!GetMesh()) { return nullptr; }

	auto AnimInstance = GetMesh()->GetAnimInstance();
	if (!AnimInstance) { return nullptr; }

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

void APlayerBase::AlignFloor()
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	//----------------------------------------
	// Ground Trace
	//----------------------------------------

	FHitResult Hit;	

	const FVector Start = GetActorLocation();
	const FVector End = Start - GetActorUpVector() * CurrentGroundTraceLength;

	DrawDebugLine(GetWorld(), Start, End,FColor::Green);
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHit = GetWorld()->LineTraceSingleByChannel(
			Hit,
			Start,
			End,
			ECC_Visibility,
			Params);

	if (!bHit)
	{
		/*MoveComp->SetMovementMode(
			MOVE_Falling);*/
		/*MoveComp->SetGravityDirection(
			FVector::DownVector);*/
		CurrentGroundTraceLength += 50.0f;
		return;
	}
	else {
		CurrentGroundTraceLength = GroundTraceLength;
	}

	//----------------------------------------
	// Normal
	//----------------------------------------

	const FVector TargetNormal =
		Hit.ImpactNormal.GetSafeNormal();

	CurrentGroundNormal = FMath::VInterpNormalRotationTo(
			CurrentGroundNormal,
			TargetNormal,
			DeltaTime,
			NormalInterpSpeed);

	//----------------------------------------
	// Walkable Angle
	//----------------------------------------

	/*const FVector UpDir =
		-MoveComp->GetGravityDirection();*/

	const float Dot =
		FVector::DotProduct(
			TargetNormal,
			FVector::UpVector);

	const float WalkableDot = FMath::Cos(
			FMath::DegreesToRadians(MaxGroundAngle));
	//UE_LOG(LogTemp, Display, TEXT("WalkableDot %.2f"), WalkableDot);
	//UE_LOG(LogTemp, Display, TEXT("Dot %.2f"), Dot);

	FVector Gravity = -CurrentGroundNormal;

	const bool Walkable = Dot >= WalkableDot;

	if (!Walkable)
	{
	/*	MoveComp->SetMovementMode(
			MOVE_Falling);*/
		//MoveComp->SetGravityDirection(FVector::DownVector);
		/*FVector SlideDir =
			FVector::VectorPlaneProject(
				FVector(0, 0, -1),
				CurrentGroundNormal).GetSafeNormal();

		AddMovementInput(
			SlideDir.GetSafeNormal(),
			SlideSpeed);*/

		Gravity = FVector::DownVector;
	}
	
	/*DrawDebugCapsule(
		GetWorld(),
		GetActorLocation(),
		WalkCapsuleHalfHeight,
		WalkCapsuleRadius,
		GetActorQuat(),
		FColor::Magenta);

	DrawDebugCapsule(
		GetWorld(),
		GetActorLocation(),
		FallCapsuleHalfHeight,
		FallCapsuleRadius,
		GetActorQuat(),
		FColor::Orange);*/


	//----------------------------------------
	// Gravity
	//----------------------------------------

	//MoveComp->SetGravityDirection(Gravity);

	//----------------------------------------
	// Rotation
	//----------------------------------------

	
}


