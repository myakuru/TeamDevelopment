
#include "DebugPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "ProjectNull/Actor/Character/Player/Robot/Robot.h"
#include "GameFramework/CharacterMovementComponent.h"

ADebugPlayerController::ADebugPlayerController():
	ARobotController(),
	bIsDebugMode(false),
	DebugMaxAcceleration(10000.0f)
{

}

void ADebugPlayerController::BeginPlay()
{
	ARobotController::BeginPlay();

	if(bIsDebugMode)
	{
		SetDebugParameter();
	}
}

void ADebugPlayerController::SetupInputComponent()
{
	ARobotController::SetupInputComponent();

	if (UEnhancedInputComponent* enhacedInput = Cast<UEnhancedInputComponent>(InputComponent))
	{
		enhacedInput->BindAction(MoveUpAction, ETriggerEvent::Triggered, this, &ADebugPlayerController::MoveUp);
		enhacedInput->BindAction(ChangeDebugModeAction, ETriggerEvent::Started, this, &ADebugPlayerController::ChangeDebugMode);
	}
}

void ADebugPlayerController::ChangeDebugMode(const FInputActionValue& ActionValue)
{
	if(bIsDebugMode)
	{
		bIsDebugMode = false;
		if (auto* player = Cast<APlayerBase>(GetCharacter()))
		{
			player->ResetMovementParameters();
		}
	}
	else
	{
		bIsDebugMode = true;
		SetDebugParameter();
	}
}

void ADebugPlayerController::SetDebugParameter()
{
	if (auto* movement = GetCharacter()->GetCharacterMovement())
	{
		movement->SetMovementMode(MOVE_Flying);
		movement->MaxAcceleration	= DebugMaxAcceleration;
		movement->MaxFlySpeed		= DebugMaxAcceleration;
	}
}

void ADebugPlayerController::MoveUp(const FInputActionValue& ActionValue)
{
	if(auto* character = GetCharacter())
	{
		const float Value = ActionValue.Get<float>();
		character->AddMovementInput(FVector::UpVector, Value);
	}
}

void ADebugPlayerController::GearExecute01(const FInputActionValue& GearActionValue01)
{
	if (bIsDebugMode) { return; }
	ARobotController::GearExecute01(GearActionValue01);
}
