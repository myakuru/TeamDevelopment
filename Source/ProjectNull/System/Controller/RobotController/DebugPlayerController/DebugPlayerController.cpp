
#include "DebugPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ProjectNull/Actor/Character/Player/Robot/Robot.h"

#include "GameFramework/CharacterMovementComponent.h"

ADebugPlayerController::ADebugPlayerController():
	ARobotController(),
	bIsDebugMode(false)
{

}

void ADebugPlayerController::BeginPlay()
{
	ARobotController::BeginPlay();
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
	FTransform transform;
	APawn* oldPawn = GetPawn();

	if (oldPawn)
	{
		transform = oldPawn->GetActorTransform();
		oldPawn->Destroy();
	}

	APawn* newPawn = nullptr;

	if (bIsDebugMode)
	{
		newPawn = GetWorld()->SpawnActor<APawn>(GameClass, transform);
		bIsDebugMode = false;
	}
	else
	{
		newPawn = GetWorld()->SpawnActor<APawn>(DebugClass, transform);
		bIsDebugMode = true;
	}

	if (newPawn)
	{
		Possess(newPawn);
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
