#include "CollisionAttack.h"
#include <ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h>

UCollisionAttack::UCollisionAttack()
	: MaxDuration(0.f)
	  , JustExecuteTime(0.f)
	  , BlinkingElement(FBlinkingHitElemental())
{
}

void UCollisionAttack::Update(const float InDeltaTime)
{
	// 有効時以外は処理しない
	if (!IsActive()) { return; }

	UAttackBase::Update(InDeltaTime);

	const float CurrentTime = GetWorld()->GetTimeSeconds();

	// 発動した瞬間から有効時間分経過していたら終了
	if (CurrentTime > MaxDuration + JustExecuteTime)
	{
		Cancel();
		return;
	}

	// 点滅判定がfalseでHIT配列がEmptyでないなら一度リセット
	if (!IsHitCheckActiveNow(CurrentTime-JustExecuteTime)&&!GetHitActors().IsEmpty())
	{
		ResetAllActors();
		UE_LOG(LogTemp,Warning,TEXT("ArraySize  : %d"),GetHitActors().Num());
	}
}

void UCollisionAttack::Execute(const FVector& InTargetLocation)
{
	SetIsActive(true); // 攻撃有効化
	SetCanExecute(false); // 攻撃実行不可にする
	JustExecuteTime = GetWorld()->GetTimeSeconds(); // 「発動した時間」として現在の時間を保存
	
	// 既に持っているアクター配列をリセット
	if (!GetHitActors().IsEmpty())
	{
		ResetAllActors();
	}
}

void UCollisionAttack::Cancel()
{
	SetIsActive(false); // 攻撃無効化
}

bool UCollisionAttack::IsHitCheckActiveNow(const float InElapsedTime) const
{
	// 点滅処理を行わないならtrueを返す
	if (!BlinkingElement.bIsBlinking) { return true; }

	// 経過時間をインターバルで割って、出力された数値が「奇数か偶数」かを取得
	// 0.0f ~ 0.4fなら「0」, 0.5f ~ 1.0fなら「1」...
	const int32 Phase = FMath::FloorToInt(InElapsedTime / BlinkingElement.BlinkInterval);
	
	// 偶数ならON
	return (Phase % 2) == 0;
}

void UCollisionAttack::OnCollisionBeginOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || !GetOwnerActor().IsValid()
		|| OtherActor == GetOwnerActor())
	{
		return;
	}

	if (!GetHitActors().Contains(OtherActor))
	{
		// キャラクターインターフェースを実装しているか
		if (auto* interface = Cast<ICharacterInterface>(OtherActor))
		{
			interface->ApplyDamaged(GetFinalDamage());
			interface->ApplyKnockBack(GetOwnerActor()->GetActorLocation());
			AddHitActors(OtherActor);

			// 攻撃がHITした瞬間のデリゲートの発火
			OnOverlapInDelegate.Broadcast(OtherActor);
		}
	}
}

void UCollisionAttack::OnCollisionEndOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex
)
{
	if (!OtherActor || !GetOwnerActor().IsValid()
		|| OtherActor == GetOwnerActor())
	{
		return;
	}

	if (GetHitActors().Contains(OtherActor))
	{
		RemoveActor(OtherActor);

		// HIT判定から抜け出した瞬間のデリゲートの発火
		OnOverlapOutDelegate.Broadcast(OtherActor);
	}
}
