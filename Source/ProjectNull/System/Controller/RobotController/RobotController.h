#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotController.generated.h"

struct FInputActionValue;

class APlayerBase;
class UInputMappingContext;

class UInputAction;

class UPlayerHUDWidget;

UCLASS()
class PROJECTNULL_API ARobotController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARobotController();

	UFUNCTION(BlueprintCallable, Category = "UI")
	inline UPlayerHUDWidget* GetPlayerHUD() const { return PlayerHUD; }


	inline void SetCanReceiveInput(bool bInCanReceiveInput) { bCanReceiveInput = bInCanReceiveInput; }

protected:

	virtual void BeginPlay()			override;
	virtual void SetupInputComponent()	override;

	virtual void GearExecute01(const FInputActionValue& GearActionValue01);


private:

	void InitializeInputContext();

	void Move(const FInputActionValue& MoveActionValue);

	void Look(const FInputActionValue& LookActionValue);
	void Jump(const FInputActionValue& JumpActionValue);

	void ChangeGear(const FInputActionValue& ActionValue);

	void InitializeUI();


	/** 入力受付可能か */
	bool bCanReceiveInput;

	UPROPERTY()
	TObjectPtr<APlayerBase> PlayerBase;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	/** ギアの変更アクション */
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> ChangeGearAction;

	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GearAction01;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> PlayerHUD;

};
