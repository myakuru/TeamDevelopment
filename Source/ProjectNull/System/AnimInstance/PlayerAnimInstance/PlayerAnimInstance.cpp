#include "PlayerAnimInstance.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

UPlayerAnimInstance::UPlayerAnimInstance():
	bIsCombatStance(false)
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
