
#include "RobotController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/Robot/Robot.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/UI/PlayerExpUpgradeWidget/PlayerExpUpgradeWidget.h>


ARobotController::ARobotController():
		bCanReceiveInput(true),
		PlayerBase(nullptr),
		InputContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr),
		JumpAction(nullptr),
		ChangeGearAction(nullptr),
		GearAction01(nullptr),
		PlayerHud(nullptr)
{
	bReplicates = true;
}

void ARobotController::OpenPlayerExpUpgradeWidget()
{
	if (PlayerExpUpgradeWidget)
	{
		PlayerExpUpgradeWidget->OpenUpgradeWidget();
	}
}

void ARobotController::BeginPlay()
{
	Super::BeginPlay();

	PlayerBase = Cast<APlayerBase>(GetCharacter());

	InitializeInputContext();

	InitializeUI();
}

void ARobotController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (!Cast<UEnhancedInputComponent>(InputComponent)) { return; }
	auto* EnhacedInput = Cast<UEnhancedInputComponent>(InputComponent);
	
	EnhacedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARobotController::Move);
	EnhacedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARobotController::Look);
	EnhacedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ARobotController::Jump);
	EnhacedInput->BindAction(ChangeGearAction, ETriggerEvent::Started, this, &ARobotController::ChangeGear);
	EnhacedInput->BindAction(GearAction01, ETriggerEvent::Started, this, &ARobotController::GearExecute01);
}

void ARobotController::InitializeInputContext()
{
	if (!InputContext) { return; }

	if (auto* SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) {
		SubSystem->AddMappingContext(InputContext, 0);
	}
}

void ARobotController::Move(const FInputActionValue& MoveActionValue)
{
	if (!bCanReceiveInput) { return; }
	if (!Cast<APlayerBase>(GetCharacter())) { return; }
	auto* ControlledPlayer = Cast<APlayerBase>(GetCharacter());

	ControlledPlayer->Move(MoveActionValue.Get<FVector2D>());
}

void ARobotController::Look(const FInputActionValue& LookActionValue)
{
	if (!bCanReceiveInput) { return; }
	const FVector2D LookVector = LookActionValue.Get<FVector2D>();
	AddYawInput(LookVector.X);
	AddPitchInput(LookVector.Y);
}

void ARobotController::Jump(const FInputActionValue& JumpActionValue)
{
	if (!bCanReceiveInput) { return; }
	if (!GetCharacter()) { return; }
	GetCharacter()->Jump();
}

void ARobotController::ChangeGear(const FInputActionValue& ActionValue)
{
	if (!bCanReceiveInput) { return; }
	if (!PlayerBase) { return; }
	PlayerBase->ChangeGear();
}

void ARobotController::GearExecute01(const FInputActionValue& GearActionValue01)
{
	if (!bCanReceiveInput) { return; }
	if (!PlayerBase || !PlayerBase->GetGearComponent()) { return; }
	PlayerBase->GetGearComponent()->ExecuteGear(0);
}

void ARobotController::InitializeUI()
{
	if (IsLocalController() && PlayerHUDClass && PlayerExpUpgradeWidgetClass)
	{
		PlayerHud					= CreateWidget<UPlayerHUDWidget>(this,PlayerHUDClass);
		PlayerExpUpgradeWidget		= CreateWidget<UPlayerExpUpgradeWidget>(this,PlayerExpUpgradeWidgetClass);

		if (PlayerHud && PlayerExpUpgradeWidget)
		{
			PlayerHud->AddToViewport();
			PlayerExpUpgradeWidget->AddToViewport();
		}
	}
}

