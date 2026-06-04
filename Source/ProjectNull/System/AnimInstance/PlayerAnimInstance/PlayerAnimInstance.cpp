#include "PlayerAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"

#include "AnimationStateMachineLibrary.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

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
	AscendingVelocityThreshold(100.f)
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

	bIsFalling	= CharacterMovement->IsFalling();
	Velocity	= CharacterMovement->Velocity;

	GroundSpeed = Velocity.Size();
	
	bIsDecelerating = (GroundSpeed != PrevGroundSpeed) && (GroundSpeed < PrevGroundSpeed);
	

	PrevGroundSpeed = GroundSpeed;

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
