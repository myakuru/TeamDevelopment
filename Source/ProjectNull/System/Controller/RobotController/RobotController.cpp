// Fill out your copyright notice in the Description page of Project Settings.


#include "RobotController.h"

#include "../../../Actor/Character/Player/Robot/Robot.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

ARobotController::ARobotController()
	:	InputContext(nullptr),
		MoveAction(nullptr),
		LookAction(nullptr),
		JumpAction(nullptr)
{
	bReplicates = true;
}

void ARobotController::BeginPlay()
{
	Super::BeginPlay();

	// 入力マッピングコンテキスト関連の初期化
	InitializeInputContext();
}

void ARobotController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* enhancedInput = Cast<UEnhancedInputComponent>(InputComponent)) 
	{
		enhancedInput->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ARobotController::Move);
		enhancedInput->BindAction(LookAction, ETriggerEvent::Triggered, this, &ARobotController::Look);
		enhancedInput->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ARobotController::Jump);
	}
}

void ARobotController::InitializeInputContext()
{
	if (!InputContext) { return; }

	if (UEnhancedInputLocalPlayerSubsystem* subSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer())) 
	{
		subSystem->AddMappingContext(InputContext, 0);
	}
}

void ARobotController::Move(const FInputActionValue& MoveActionValue)
{
	const FVector2d inputVector = MoveActionValue.Get<FVector2D>();
	const FRotator	yawRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	const FVector forward	= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::X);
	const FVector right		= FRotationMatrix(yawRotation).GetUnitAxis(EAxis::Y);


	if (APawn* controlledPawn = GetPawn())
	{
		controlledPawn->AddMovementInput(forward, inputVector.Y);
		controlledPawn->AddMovementInput(right, inputVector.X);
	}
}

void ARobotController::Look(const FInputActionValue& LookActionValue)
{
	const FVector2D lookVector = LookActionValue.Get<FVector2D>();
	AddYawInput(lookVector.X);
	AddPitchInput(lookVector.Y);
}

void ARobotController::Jump(const FInputActionValue& JumpActionValue)
{
	if (ACharacter* controlledCharacter = GetCharacter())
	{
		controlledCharacter->Jump();
	}
}

