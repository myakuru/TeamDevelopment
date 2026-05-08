#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotController.generated.h"

struct FInputActionValue;

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
	UPlayerHUDWidget* GetPlayerHUD() const { return PlayerHUD; }

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

	UPROPERTY()
	TObjectPtr<APlayerBase> PlayerBase;


	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputMappingContext> InputContext;

	// �ړ����̓A�N�V����
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> MoveAction;

	// ���_���̓A�N�V����
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> LookAction;

	// �W�����v���̓A�N�V����
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> JumpAction;

	// �M�A���̓A�N�V���� 01
	UPROPERTY(EditAnywhere, Category = "Input")
	TObjectPtr<UInputAction> GearAction01;

	// �v���C���[HUD�E�B�W�F�b�g�N���X
	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UPlayerHUDWidget> PlayerHUDClass;

	// �v���C���[HUD�E�B�W�F�b�g�̃C���X�^���X
	UPROPERTY()
	TObjectPtr<UPlayerHUDWidget> PlayerHUD;

};
