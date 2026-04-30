#include "TreasureBox.h"

ATreasureBox::ATreasureBox()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATreasureBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (OnHitDistancePlayer())
	{
		Destroy();
	}
}
