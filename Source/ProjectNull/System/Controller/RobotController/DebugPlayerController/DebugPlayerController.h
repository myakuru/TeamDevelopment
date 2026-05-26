
#pragma once

#include "CoreMinimal.h"
#include "../RobotController.h"
#include "DebugPlayerController.generated.h"

class ARobot;

UCLASS()
class PROJECTNULL_API ADebugPlayerController : public ARobotController
{
	GENERATED_BODY()
	
public:

	ADebugPlayerController();

private:

	virtual void BeginPlay()			override;
	virtual void SetupInputComponent()	override;

	void ChangeDebugMode(const FInputActionValue& ActionValue);

	void MoveUp(const FInputActionValue& ActionValue);


	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> MoveUpAction;

	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> ChangeDebugModeAction;

	bool bIsDebugMode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> GameClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> DebugClass;
};
