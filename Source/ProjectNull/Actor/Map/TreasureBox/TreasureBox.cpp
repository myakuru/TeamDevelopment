#include "TreasureBox.h"

ATreasureBox::ATreasureBox()
{
	// イベント登録
	Trigger->OnComponentBeginOverlap.AddDynamic(
		this,
		&AMapActorBase::HitReaction
	);
}

void ATreasureBox::HitReaction(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APawn* Pawn = Cast<APawn>(OtherActor);
	if (!Pawn) return;

	// プレイヤーだけにしたい場合
	if (!Pawn->IsPlayerControlled()) return;

	Destroy();
}
