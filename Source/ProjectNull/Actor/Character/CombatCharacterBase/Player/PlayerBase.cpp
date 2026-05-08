#include "PlayerBase.h"

#include "Camera/CameraComponent.h"
#include <GameFramework/SpringArmComponent.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <GameFramework/CharacterMovementComponent.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/System/Combat/Attack/AutoAttack/AutoAttack.h>
#include <ProjectNull/System/Subsystem/WorldSubsystem/EnemyManagerSubsystem/EnemyManagerSubsystem.h>
#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/Data/CharacterParameterData/PlayerParameterData/PlayerParameterData.h>
#include <ProjectNull/Data/CharacterRuntimeData/PlayerRuntimeData/PlayerRuntimeData.h>


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

	if (GearComponent) {
		GearComponent->SetOwnerPlayer(this);
	}

	if (AutoAttack) {
		AutoAttack->Initialize(this);
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

int32 APlayerBase::GetCurrentGearLevel() const
{
	if (!GearComponent) { return 0; }
	return GearComponent->GetCurrentGearLevel();
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
