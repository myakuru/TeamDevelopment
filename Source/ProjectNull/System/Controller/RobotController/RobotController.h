#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotController.generated.h"

struct FInputActionValue;

class APlayerBase;
class UInputMappingContext;
class UInputAction;
class UPlayerHUDWidget;
class UPlayerExpUpgradeWidget;

UCLASS()
class PROJECTNULL_API ARobotController : public APlayerController
{
	GENERATED_BODY()
	
public:

	ARobotController();

	void OpenPlayerExpUpgradeWidget();

	UFUNCTION(BlueprintCallable, Category = "UI")
	inline UPlayerHUDWidget* GetPlayerHUD() const { return PlayerHud; }

	/** Setter */
	inline void SetCanReceiveInput(bool bInCanReceiveInput) { bCanReceiveInput = bInCanReceiveInput;}
	inline bool HasMoveInput() const { return bHasMoveInput; }
	

protected:

	virtual void BeginPlay()			override;
	virtual void SetupInputComponent()	override;



private:

	void InitializeInputContext();

	void Move(const FInputActionValue& MoveActionValue);

	void Look(const FInputActionValue& LookActionValue);
	void Jump(const FInputActionValue& JumpActionValue);

	void ChangeGear(const FInputActionValue& ActionValue);

	void InitializeUI();


	void GearExecute(
		const FInputActionValue& ActionValue,
		int32 ExecuteIndex);



	/** 入力受付可能か */
	bool bCanReceiveInput;

	/** 移動入力が存在するか */
	UPROPERTY()
	bool bHasMoveInput;

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
	TArray<TObjectPtr<UInputAction>> GearActions;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerExpUpgradeWidget> PlayerExpUpgradeWidgetClass;

	TObjectPtr<UPlayerHUDWidget> PlayerHud;
	TObjectPtr<UPlayerExpUpgradeWidget> PlayerExpUpgradeWidget;

};
