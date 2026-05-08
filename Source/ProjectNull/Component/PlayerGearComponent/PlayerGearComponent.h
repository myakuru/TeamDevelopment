#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerGearComponent.generated.h"

class UGearBase;

class APlayerBase;


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerGearComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerGearComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	bool IsMovementBlockedByGear() const;

	void ExecuteGear(int32 GearIndex);

	void ChangeGear();

	/** セッター */
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

	/** ゲッター */
	inline const TArray<UGearBase*>& GetPlayerGears() const { return PlayerGears; }
	inline int32 GetCurrentGearLevel() const				{ return CurrentGearLevel; }

private:

	bool CanChangeGear() const;

	UPROPERTY()
	APlayerBase* OwnerPlayer;

	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearBase*> PlayerGears;

	UPROPERTY(EditAnywhere)
	int32 CurrentGearLevel;
};
