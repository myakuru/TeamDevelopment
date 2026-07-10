//----------------------------------------------------------
//		Includes
//----------------------------------------------------------
#include "DamageNumberPoolSubsystem.h"

#include <ProjectNull/UI/InGame/HitDamageWidget/DamageNumberActor/DamageNumberActor.h>
#include "Engine/World.h"

//----------------------------------------------------------
//		Poolの初期化
//----------------------------------------------------------
void UDamageNumberPoolSubsystem::InitializePool(
	TSubclassOf<ADamageNumberActor> InDamageNumberClass,
	int32 InitialPoolSize
)
{
	if (!GetWorld())
	{
		return;
	}

	DamageNumberClass = InDamageNumberClass;

	if (!DamageNumberClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumberClass is null."));
		return;
	}

	// 二重初期化防止
	if (bInitialized)
	{
		return;
	}

	// DamageNumberActorクラスを、PoolSize分だけ確保して初期化
	for (int32 i = 0; i < InitialPoolSize; ++i)
	{
		// アクターの生成
		ADamageNumberActor* NewActor = GetWorld()->SpawnActor<ADamageNumberActor>(
			DamageNumberClass,
			FVector::ZeroVector,
			FRotator::ZeroRotator
		);

		// 生成に成功したら初期化・自分の無効化リストに追加
		if (NewActor)
		{
			NewActor->DeactivateDamageNumber();
			InactiveActors.Add(NewActor);
		}
	}

	bInitialized = true;

	UE_LOG(LogTemp, Log, TEXT("DamageNumberPool initialized. Size: %d"), InitialPoolSize);
}

//----------------------------------------------------------
//		表示リストに追加
//----------------------------------------------------------
void UDamageNumberPoolSubsystem::ShowDamageNumber(
	const FVector& WorldLocation,
	int32 Damage,
	bool bCritical
)
{
	// Worldの有無チェック
	if (!GetWorld())
	{
		return;
	}

	// DamageNumberClassの有無チェック
	if (!DamageNumberClass)
	{
		UE_LOG(LogTemp, Warning, TEXT("DamageNumberClass is not set. Call InitializePool first."));
		return;
	}

	// PoolからActorの取得
	ADamageNumberActor* DamageActor = GetPooledActor();

	// 取得に失敗
	if (!DamageActor)
	{
		return;
	}

	// 表示有効化リストに追加
	ActiveActors.Add(DamageActor);
	DamageActor->ActivateDamageNumber(WorldLocation, Damage, bCritical, this);
}

//----------------------------------------------------------
//		表示の終了(Actor側から呼び出される)
//----------------------------------------------------------
void UDamageNumberPoolSubsystem::ReleaseDamageNumber(ADamageNumberActor* DamageNumberActor)
{
	if (!DamageNumberActor)
	{
		return;
	}

	DamageNumberActor->DeactivateDamageNumber();

	ActiveActors.RemoveSwap(DamageNumberActor);
	InactiveActors.Add(DamageNumberActor);
}

//----------------------------------------------------------
//		PoolからActorの取得
//----------------------------------------------------------
ADamageNumberActor* UDamageNumberPoolSubsystem::GetPooledActor()
{
	// 余っているActorがあれば再利用
	if (InactiveActors.Num() > 0)
	{
		ADamageNumberActor* Actor = InactiveActors.Pop();

		if (IsValid(Actor))
		{
			return Actor;
		}
	}

	// 足りなければ追加生成
	if (!DamageNumberClass || !GetWorld())
	{
		return nullptr;
	}

	ADamageNumberActor* NewActor = GetWorld()->SpawnActor<ADamageNumberActor>(
		DamageNumberClass,
		FVector::ZeroVector,
		FRotator::ZeroRotator
	);

	if (NewActor)
	{
		NewActor->DeactivateDamageNumber();
	}

	return NewActor;
}