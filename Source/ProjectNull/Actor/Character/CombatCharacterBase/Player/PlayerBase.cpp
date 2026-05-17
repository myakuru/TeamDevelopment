#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>

#include "Animation/AnimSingleNodeInstance.h"
#include "Animation/AnimationAsset.h"
#include "Components/SkeletalMeshComponent.h"

#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>
#include <ProjectNull/System/AnimInstance/PlayerAnimInstance/PlayerAnimInstance.h>
#include <ProjectNull/Actor/Effect/ModelAfterimageTrailEffect/ModelAfterimageTrailEffect.h>


APlayerBase::APlayerBase()
	:	SpringArmComponent(nullptr),
		CameraComponent(nullptr),
		AutoAttack(nullptr),
		GearComponent(nullptr)
{
	// ================================================================
	// プレイヤーの初期化
	// ================================================================

	PrimaryActorTick.bCanEverTick = true;
	bUseControllerRotationYaw = false;

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

	GearComponent = CreateDefaultSubobject<UPlayerGearComponent>("Gear");
}

void APlayerBase::BeginPlay()
{
	ACombatCharacterBase::BeginPlay();

	Instance = GetWorld()->GetGameInstance<USuperGameInstance>();

	if (AutoAttack) {
		AutoAttack->Initialize(this);
	}

	if (ModelAfterimageTrailEffect) {
		ModelAfterimageTrailEffect->SetOwnerActor(this);
		ModelAfterimageTrailEffect->SetCanAddTrailPoint(true);
	}

	UpdateHUDHP();
}

void APlayerBase::Tick(float DeltaTime)
{
	UEnemyManagerSubsystem* enemyManager = GetWorld()->GetSubsystem<UEnemyManagerSubsystem>();
	if (!enemyManager) { return; }

	
	ACombatCharacterBase::Tick(DeltaTime);

	if (AutoAttack) {
		AutoAttack->Update(DeltaTime,nullptr,enemyManager);
	}

	//Main Status

	/*UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	int32 MachineIndex =
		AnimInstance->GetStateMachineIndex(TEXT("Main Status"));

	FName StateName =
		AnimInstance->GetCurrentStateName(MachineIndex);

	float CurrentTime =
		AnimInstance->GetRelevantAnimTime(MachineIndex, );*/

	if (ModelAfterimageTrailEffect) {

		if (!GetMesh()->GetSingleNodeInstance()) { return; }
		//UE_LOG(LogTemp, Warning, TEXT("hi ssss"));
		ModelAfterimageTrailEffect->Update(DeltaTime, GetMesh()->GetSkeletalMeshAsset(),
			GetMesh()->GetSingleNodeInstance()->GetAnimationAsset(), GetMesh()->GetPosition());
	}
	
	
	if (ARobotController* RobotController = Cast<ARobotController>(GetController())) {
		HUDWidget = RobotController->GetPlayerHUD();
	}
}

void APlayerBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACombatCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void APlayerBase::Move(const FVector2d& InputVector)
{
	if (!CanMove()) { return; }

	const FRotator YawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

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

bool APlayerBase::CanMove()
{
	if(GearComponent && GearComponent->IsMovementBlockedByGear()) {
		return false;
	}

	return true;
}

void APlayerBase::UpdateHUDHP()
{
	if (HUDWidget) {
		//HUDWidget->SetPlayerHp(CombatStats.HP.Current, CombatStats.HP.Max);
	}
}
