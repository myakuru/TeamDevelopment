
#pragma once

#include "CoreMinimal.h"
#include "../Robot/Robot.h"
#include "DebugFlyPawn.generated.h"

class UFloatingPawnMovement;

UCLASS()
class PROJECTNULL_API ADebugFlyPawn : public ARobot
{
	GENERATED_BODY()

public:
	ADebugFlyPawn();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
private:

	// 移動コンポーネント
	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* Movement;

	void MoveUp(float Value);

	
};
