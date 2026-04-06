#include "DebugFlyPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ADebugFlyPawn::ADebugFlyPawn()
	: ARobot()
{
	// ˆÚ“®
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

	// ‘¬“xÝ’è
	NormalSpeed = 1200.0f;
	BoostSpeed = 5000.0f;

	Movement->MaxSpeed = NormalSpeed;
}

void ADebugFlyPawn::BeginPlay()
{
	ARobot::BeginPlay();
}

void ADebugFlyPawn::Tick(float DeltaTime)
{
	ARobot::Tick(DeltaTime);
}

void ADebugFlyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ARobot::SetupPlayerInputComponent(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveUp", this, &ADebugFlyPawn::MoveUp);
	PlayerInputComponent->BindAction("Boost", IE_Pressed, this, &ADebugFlyPawn::StartBoost);
	PlayerInputComponent->BindAction("Boost", IE_Released, this, &ADebugFlyPawn::StopBoost);
}

void ADebugFlyPawn::MoveUp(float Value)
{
	AddMovementInput(FVector::UpVector, Value);
}

void ADebugFlyPawn::StartBoost()
{
	Movement->MaxSpeed = BoostSpeed;
}

void ADebugFlyPawn::StopBoost()
{
	Movement->MaxSpeed = NormalSpeed;
}