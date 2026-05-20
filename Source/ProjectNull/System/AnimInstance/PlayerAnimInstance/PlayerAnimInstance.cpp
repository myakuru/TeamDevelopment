#include "PlayerAnimInstance.h"

UPlayerAnimInstance::UPlayerAnimInstance():
	bIsCombatStance(false)
{

}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	UAnimInstance::NativeUpdateAnimation(DeltaSeconds);

	SavePoseSnapshot();
}

void UPlayerAnimInstance::SavePoseSnapshot()
{
	//UE_LOG(LogTemp, Warning, TEXT("hi ssss"));

	SnapshotPose(PlayerPoseSnapshot);
}
