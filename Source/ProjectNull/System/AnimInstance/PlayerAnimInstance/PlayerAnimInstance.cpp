#include "PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AnimationStateMachineLibrary.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/System/Controller/RobotController/RobotController.h>

UPlayerAnimInstance::UPlayerAnimInstance():
	bShouldMove(false),
	bIsFalling(false),
	bIsCombatStance(false),
	bIsDecelerating(false),
	Velocity(FVector::ZeroVector),
	GroundSpeed(0.f),
	PrevGroundSpeed(0.f),
	PlayerPoseSnapshot(FPoseSnapshot()),
	Player(nullptr),
	MoveThresholdSpeed(3.f),
	AscendingVelocityThreshold(100.f),
	RunStopSpeedThreshold(100.f),
	bShouldEnterRunStop(false)
{

}

void UPlayerAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Player = Cast<APlayerBase>(GetOwningActor());
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	
	if (!Player) { return; }

	auto CharacterMovement = Player->GetCharacterMovement();
	if (!CharacterMovement) { return; }

	auto Controller = Player->GetController();
	auto PlayerController = Cast<ARobotController>(Controller);

	if (!PlayerController) { return; }
	bHasMoveInput = PlayerController->HasMoveInput();
	PrevGroundSpeed = GroundSpeed;
	bIsFalling		= CharacterMovement->IsFalling();
	Velocity		= CharacterMovement->Velocity;
	
	GroundSpeed = Velocity.Size();
	
	bIsDecelerating = (GroundSpeed != PrevGroundSpeed) && (GroundSpeed < PrevGroundSpeed);
	
	bShouldEnterRunStop = GroundSpeed >= RunStopSpeedThreshold;

	//UE_LOG(LogTemp, Display, TEXT("GroundSpeed %.2f"), GroundSpeed);
	bShouldMove = GroundSpeed > MoveThresholdSpeed;
}

FPoseSnapshot& UPlayerAnimInstance::GetPlayerPoseSnapshot()
{
	SavePoseSnapshot();
	return PlayerPoseSnapshot;
}

void UPlayerAnimInstance::SavePoseSnapshot()
{
	SnapshotPose(PlayerPoseSnapshot);
}
