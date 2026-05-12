#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearBase.generated.h"


USTRUCT(BlueprintType)
struct FGearStatus
{
	GENERATED_BODY()
public:

	FGearStatus():
		Duration(0.0f),
		CoolTime(0.0f)
	{
	
	}
public:

	UPROPERTY(EditAnywhere)
	float Duration;

	UPROPERTY(EditAnywhere)
	float CoolTime;
};


class UPlayerGearComponent;

class APlayerBase;

class UGearStateBase;


UCLASS()
class PROJECTNULL_API UGearBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearBase();

public:

	static constexpr int32 kMaxGearLevel = 4;

	virtual void Initialize(class APlayerBase* Player,class UPlayerGearComponent* GearComponent);

	virtual void Execute(int32 CurrentGearLevel);

	virtual void Update(float DeltaTime);

	/** ゲッター */
	inline float GetElapsedTime()		const		{ return ElapsedTime; }
	inline bool CanExecute()			const		{ return bCanExecute; }
	inline bool BlocksMovement()		const		{ return bBlocksMovement; }
	inline bool IsActive()				const		{ return bIsActive; }
	inline bool IsMovementBlocked()		const		{ return bBlocksMovement; }

	/** セッター */
	inline void SetBlocksMovement(bool Flg)	{ bBlocksMovement = Flg; }
	inline void SetCanExecute(bool Flg)		{ bCanExecute = Flg; }
	inline void SetGearDuration(float Duration,int32 Index) { 
		if (!GearStatuses.IsValidIndex(Index)) { return; }
		GearStatuses[Index].Duration = Duration;
	}


protected:

	UPROPERTY()
	APlayerBase* OwnerPlayer;

	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

private:

	virtual void ResetParams();


	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearStateBase*> GearStates;

	UPROPERTY(EditAnywhere)
	TArray<FGearStatus> GearStatuses;

	int32 ExecutedGearLevel;

	bool bCanExecute;

	float ElapsedTime;

	bool bIsActive;

	bool bBlocksMovement;

	FTimerHandle DurationTimerHandle;
};
