
#include "GroundAlignmentComponent.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Utility/GroundUtility/GroundUtility.h>


UGroundAlignmentComponent::UGroundAlignmentComponent():
	RootComp(nullptr)
{
	PrimaryComponentTick.bCanEverTick = true;
	RootComp = CreateDefaultSubobject<USceneComponent>("Root");
	//SetRootComponent(RootComp);

}


void UGroundAlignmentComponent::BeginPlay()
{
	Super::BeginPlay();

	
}


void UGroundAlignmentComponent::TickComponent(
	float DeltaTime,
	ELevelTick TickType,
	FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(
		DeltaTime,
		TickType,
		ThisTickFunction);

	auto Player = Cast<APlayerBase>(GetOwner());
	if (!Player) { return; }

	FVector FloorNormal = FVector::ZeroVector;
	if (!Player->GetCurrentFloorNormal(FloorNormal)) { return; }

	if (FloorNormal.IsNearlyZero()) {
		FloorNormal = FVector::UpVector;
	}

	const FQuat TargetQuat = UGroundUtility::MakeRotationFromGroundNormal(
		Player->GetActorTransform(),
		FloorNormal);

	if (!RootComp) { return; }
	RootComp->SetWorldRotation(TargetQuat);
}

