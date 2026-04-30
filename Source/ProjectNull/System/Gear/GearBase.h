#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearBase.generated.h"


// �M�A�̃X�e�[�^�X�\����
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

	// �M�A��������
	UPROPERTY(EditAnywhere)
	float Duration;

	// �M�A�̃N�[���^�C��
	UPROPERTY(EditAnywhere)
	float CoolTime;
};


// �v���C���[�̃M�A�R���|�[�l���g�N���X
class UPlayerGearComponent;

// �v���C���[�̒��Ԋ��N���X
class APlayerBase;

// �M�A�̏�Ԃ̊��N���X
class UGearStateBase;


/// <summary>
/// �M�A�̊��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UGearBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearBase();

public:

	// �M�A�̍ő僌�x��
	static constexpr int32 kMaxGearLevel = 4;

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize(class APlayerBase* Player,class UPlayerGearComponent* GearComponent);

	/// <summary>
	/// �M�A�̎��s
	/// </summary>
	virtual void Execute(int32 CurrentGearLevel);

	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	virtual void Update(float DeltaTime);

	// �Q�b�^�[
	bool CanExecute()			const { return bCanExecute; }
	bool BlocksMovement()		const { return bBlocksMovement; }
	bool IsActive()				const { return bIsActive; }
	bool IsMovementBlocked()	const { return bBlocksMovement; }

	// �Z�b�^�[
	void SetBlocksMovement(bool Flg)	{ bBlocksMovement = Flg; }
	void SetCanExecute(bool Flg)		{ bCanExecute = Flg; }

protected:

	// ������̃v���C���[�̃|�C���^
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// ������M�A�R���|�[�l���g�̃|�C���^
	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

private:

	virtual void ResetParams();


	// �M�A�̏�Ԃ̔z��
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearStateBase*> GearStates;

	// �M�A�̃X�e�[�^�X�̔z��
	UPROPERTY(EditAnywhere)
	TArray<FGearStatus> GearStatuses;

	// ���s���ꂽ�M�A�̃��x��
	int32 ExecutedGearLevel;

	// �M�A����s�ł��邩�ǂ���
	bool bCanExecute;

	// �o�ߎ���
	float ElapsedTime;

	// �M�A�����s�����ǂ���
	bool bIsActive;

	// �M�A�ɂ���Ĉړ���u���b�N���邩�ǂ���
	bool bBlocksMovement;

	// �M�A�̎������Ԃ�Ǘ�����^�C�}�[�n���h��
	FTimerHandle DurationTimerHandle;
};
