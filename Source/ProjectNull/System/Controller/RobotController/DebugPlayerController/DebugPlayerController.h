
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
	virtual void GearExecute01(const FInputActionValue& GearActionValue01) override;

	// �f�o�b�O���[�h�̐؂�ւ��֐�
	void ChangeDebugMode(const FInputActionValue& ActionValue);

	/// <summary>
	/// ������ւ̈ړ����͏����֐�
	/// </summary>
	/// <param name="ActionValue">���͒l</param>
	void MoveUp(const FInputActionValue& ActionValue);


	// ������ւ̈ړ����̓A�N�V����
	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> MoveUpAction;

	// �f�o�b�O���[�h�؂�ւ����̓A�N�V����
	UPROPERTY(EditAnywhere, Category = "DebugInput")
	TObjectPtr<UInputAction> ChangeDebugModeAction;

	// �f�o�b�O���[�h���ǂ����̃t���O
	bool bIsDebugMode;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> GameClass;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ARobot> DebugClass;
};
