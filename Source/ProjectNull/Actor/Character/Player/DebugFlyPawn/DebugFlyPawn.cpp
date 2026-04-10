#include "DebugFlyPawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/FloatingPawnMovement.h"

ADebugFlyPawn::ADebugFlyPawn()
	: ARobot()
{
	// ˆÚ“®
	Movement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("Movement"));

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
}

void ADebugFlyPawn::MoveUp(float Value)
{
	AddMovementInput(FVector::UpVector, Value);
}
