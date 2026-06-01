
#include "CrossLaserbeam.h"

#include "Components/BoxComponent.h"

ACrossLaserbeam::ACrossLaserbeam()
{
	PrimaryActorTick.bCanEverTick = false;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(RootComponent);

	LaserBoxes.SetNum(4);
	LaserRelativeTransforms.SetNum(4);

	for (int32 Index = 0; Index < 4; ++Index)
	{
		auto& Box = LaserBoxes[Index];

		FString Name = FString::Printf(
			TEXT("LaserBox_%d"),
			Index);

		Box = CreateDefaultSubobject<UBoxComponent>(*Name);
		if (!Box) { continue; }
		Box->SetupAttachment(RootComponent);
		//Box->SetRelativeTransform(RelativeTransform);
	}
}

void ACrossLaserbeam::BeginPlay()
{
	Super::BeginPlay();
	/*for (int32 Index = 0; Index < 4; ++Index)
	{
		LaserBoxes[Index]->SetRelativeTransform(LaserRelativeTransforms[Index]);
	}*/
}

void ACrossLaserbeam::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

