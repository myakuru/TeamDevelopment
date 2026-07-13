#pragma once

//----------------------------------------------------------
// Includes
//----------------------------------------------------------
#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "DamageNumberPoolSubsystem.generated.h"

//----------------------------------------------------------
// Forward Declaration
//----------------------------------------------------------
class ADamageNumberActor;

//----------------------------------------------------------
// UDamageNumberPoolSubsystem class
//----------------------------------------------------------
UCLASS()
class PROJECTNULL_API UDamageNumberPoolSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:

	// GameModeなどから最初に呼ぶ
	UFUNCTION(BlueprintCallable)
	void InitializePool(TSubclassOf<ADamageNumberActor> InDamageNumberClass, int32 InitialPoolSize = 64);

	// ダメージUIを表示
	UFUNCTION(BlueprintCallable)
	void ShowDamageNumber(
		const FVector& WorldLocation,
		int32 Damage,
		bool bCritical = false
	);

	// DamageNumberActorから呼ばれる
	void ReleaseDamageNumber(ADamageNumberActor* DamageNumberActor);

private:

	ADamageNumberActor* GetPooledActor();

private:

	UPROPERTY()
	TSubclassOf<ADamageNumberActor> DamageNumberClass;

	UPROPERTY()
	TArray<TObjectPtr<ADamageNumberActor>> InactiveActors;

	UPROPERTY()
	TArray<TObjectPtr<ADamageNumberActor>> ActiveActors;

	bool bInitialized = false;
};
