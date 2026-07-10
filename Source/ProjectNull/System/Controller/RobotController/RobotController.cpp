
#include "RobotController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>
#include <ProjectNull/System/Gear/GearBase.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/Robot/Robot.h>
#include <ProjectNull/Component/PlayerGearComponent/PlayerGearComponent.h>
#include <ProjectNull/UI/PlayerHUDWidget/PlayerHUDWidget.h>
#include <ProjectNull/UI/PlayerExpUpgradeWidget/PlayerExpUpgradeWidget.h>


ARobotController::ARobotController():
		bCanReceiveInput(true),
		bHasMoveInput(true),
		PlayerBase(nullptr),
		InputContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr),
		JumpAction(nullptr),
		ChangeGearAction(nullptr),
		GearActions(TArray<TObjectPtr<UInputAction>>()),
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

	auto* EnhacedInput = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhacedInput) { return; }

	
	EnhacedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARobotController::Move);
	EnhacedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARobotController::Look);
	EnhacedInput->BindAction(JumpAction, ETriggerEvent::Started, this, &ARobotController::Jump);
	EnhacedInput->BindAction(ChangeGearAction, ETriggerEvent::Started, this, &ARobotController::ChangeGear);

	for (int32 Index = 0; Index < GearActions.Num(); ++Index)
	{
		EnhacedInput->BindAction(
			GearActions[Index],
			ETriggerEvent::Started,
			this,
			&ARobotController::GearExecute,
			Index);
	}
}

void ARobotController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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
	if (!bCanReceiveInput)		{ return; }
	if (!PlayerBase)			{ return; }

	const FVector2D MoveInput = MoveActionValue.Get<FVector2D>();

	PlayerBase->Move(MoveInput);
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
	if (!bCanReceiveInput)	{ return; }
	if (!PlayerBase)		{ return; }

	PlayerBase->Jump();
}

void ARobotController::ChangeGear(const FInputActionValue& ActionValue)
{
	if (!bCanReceiveInput)	{ return; }
	if (!PlayerBase)		{ return; }

	PlayerBase->ChangeGear();
}

void ARobotController::InitializeUI()
{
	if (IsLocalController() && PlayerHUDClass && PlayerExpUpgradeWidgetClass)
	{
		PlayerHud					= CreateWidget<UPlayerHUDWidget>(this,PlayerHUDClass);
		PlayerExpUpgradeWidget		= CreateWidget<UPlayerExpUpgradeWidget>(this,PlayerExpUpgradeWidgetClass);

		if (PlayerHud && PlayerExpUpgradeWidget)
		{
			PlayerHud->AddToViewport(InGameDefinition::PlayerUIZOrder);
			PlayerExpUpgradeWidget->AddToViewport(InGameDefinition::GetGearUIZOrder);
		}
	}
}

void ARobotController::GearExecute(
	const FInputActionValue& ActionValue,
	int32 ExecuteIndex)
{
	if (!bCanReceiveInput)	{ return; }
	if (!PlayerBase)		{ return; }
	const auto GearComp = PlayerBase->GetGearComponent();
	if (!GearComp)			{ return; }

	GearComp->ExecuteGear(ExecuteIndex);
}


