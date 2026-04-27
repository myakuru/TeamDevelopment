#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "RobotController.generated.h"

// ���̓A�N�V�����̒l��\���\����
struct FInputActionValue;

// ���̓}�b�s���O�R���e�L�X�g�N���X
class UInputMappingContext;

// ���̓A�N�V�����N���X
class UInputAction;


class UPlayerHUDWidget;

/// <summary>
/// ���{�b�g�i�v���C���[�j�̃R���g���[���[�N���X
/// ���{�b�g�̑������͏�����S������N���X
/// </summary>
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

	/// <summary>
	/// �M�A�̎��s�֐� 01
	/// </summary>
	/// <param name="GearActionValue01">���͒l</param>
	virtual void GearExecute01(const FInputActionValue& GearActionValue01);

private:

	/// <summary>
	/// ���̓}�b�s���O�R���e�L�X�g�֘A�̏�����
	/// </summary>
	void InitializeInputContext();

	/// <summary>
	/// �ړ����͏����֐�
	/// </summary>
	/// <param name="MoveActionValue">���͒l</param>
	void Move(const FInputActionValue& MoveActionValue);

	/// <summary>
	/// ���_���͏����֐�
	/// </summary>
	/// <param name="LookActionValue">���͒l</param>
	void Look(const FInputActionValue& LookActionValue);

	/// <summary>
	/// �W�����v���͏����֐�
	/// </summary>
	/// <param name="LookActionValue">���͒l</param>
	void Jump(const FInputActionValue& JumpActionValue);

	/// <summary>
	/// UI�̏������֐�
	/// </summary>
	void InitializeUI();

	// ���̓}�b�s���O�R���e�L�X�g
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
