#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance():
	bIsCombatStance(false)
{

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);

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
