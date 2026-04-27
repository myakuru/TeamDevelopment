
#pragma once

#include "CoreMinimal.h"
#include "../../../../System/DataTable/KnockBackData/KnockBackData.h"
#include "../CombatCharacterBase.h"
#include "EnemyBase.generated.h"

// �O���錾
class UCapsuleComponent;
class UPrimitiveComponent;

/// <summary>
/// �G�X�e�[�^�X�̃X�P�[�����O���
/// </summary>
USTRUCT(BlueprintType)
struct FStatScaling
{
	GENERATED_BODY()

public:
	// ��b���l
	UPROPERTY(EditAnywhere)
	int32 Base = 100;

	// �{��
	UPROPERTY(EditAnywhere)
	float Scale = 1.0f;

	// �{��������
	UPROPERTY(EditAnywhere)
	float ScalePerKill = 0.005f;

	/// <summary>
	/// ��b���l * �{��
	/// </summary>
	/// <returns>�ŏI�I�Ȑ��l��Ԃ�</returns>
	int32 GetFinalValue(int32 Count)
	{
		Scale = 1.0f + Count * ScalePerKill;
		return static_cast<int32>(Base * Scale);
	}
};

/// <summary>
/// �G��{�X�e�[�^�X
/// </summary>
USTRUCT(BlueprintType)
struct FEnemyStatus
{
	GENERATED_BODY()

public:
	// �ړ�����
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	// �ړ����x
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	// ��]��ԑ��x
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;

	// �ŏI�I�ȃq�b�g�|�C���g
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	// �X�P�[�����O�v�Z�p�q�b�g�|�C���g
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	// �ŏI�I�ȍU����
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

	// �X�P�[�����O�v�Z�p�U����
	UPROPERTY(EditAnywhere)
	FStatScaling AttackScaling;

	// �G�l�~�[�̏d��
	UPROPERTY(EditAnywhere)
	float	KnockBackWeight = 1.0f;

	// �m�b�N�o�b�N����
	FVector KNockBackVelocity = FVector::ZeroVector;

	// �G�l�~�[��������ђ��̔���t���O
	bool	KnockBackFlg = false;

	// �o���l
	UPROPERTY(EditAnywhere)
	int EXP = 0;

	// �M�A�G�l���M�[
	UPROPERTY(EditAnywhere)
	int GearEnergy = 0;

	// �v���C���[�Ƃ̋���
	float DistancePlayer = 0.0f;

	// �U���\����
	UPROPERTY(EditAnywhere)
	float AttackDistance = 20.0f;

	// �U���\�t���O
	bool CanAttack = false;
};

// �G�Ǘ��N���X
class UEnemyManagerSubsystem;

// �Q�[���̐i�s�Ǘ��N���X
class UGameProgressSubsystem;

// �G�U���R���|�[�l���g
class UEnemyAttackComponent;

/// <summary>
/// �G�̒��Ԋ��N���X
/// �����FCharacter�N���X��p�����Ă��邪�R���|�[�l���g�������A
///	�d���Ȃ�\�������邽��Actor��p������\����
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemyBase : public ACombatCharacterBase
{
	GENERATED_BODY()
	
public:
	AEnemyBase();
public:

	/// <summary>
	/// �G�i���g�j��������΂���鏈��
	/// </summary>
	virtual void SetKnockBackData(const FVector& playerLocation, float AttackPower, float EnemyWeight);

	/// <summary>
	/// �G�i���g) ���_���[�W��󂯂鏈��
	/// </summary>
	virtual void SetTakeDamaged(int32 AttackPower = 1);

protected:
	virtual void BeginPlay() override;

	/// <summary>
	/// �G�i���g�j���v���C���[�֌���������
	/// </summary>
	/// <param name="playerLocation"> �v���C���[�̍��W</param>
	virtual void MoveToPlayer(const FVector& PlayerLocation, float DeltaTime);

	/// <summary>
	/// �G�i���g�j�̃p�����[�^��X�V����
	/// </summary>
	virtual void UpdateParams();

	/// <summary>
	/// SphereCollision��擾���Ďg�����߂̊֐�
	/// <summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleCollision;

	//// ����Actor�Əd�Ȃ����ۂɌĂ΂��֐�
	//UFUNCTION()
	//virtual void OnOverlap(
	//	UPrimitiveComponent* OverlappedComponent,	// �������̃R���W����
	//	AActor* OtherActor,							// �G��Ă����A�N�^
	//	UPrimitiveComponent* OtherComp,				// �G��Ă������̃R���|�[�l���g
	//	int32 OtherBodyIndex,						// �{�f�B�ԍ��i��{�g��Ȃ��j
	//	bool bFromSweep,							// Sweep���ǂ���
	//	const FHitResult& SweepResult				// ������̏ڍה���
	//);

	/// <summary>
	/// �G��������΂���Ă����Ԃ̏���
	/// </summary>
	virtual void MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime);

	// DataTable �Q��
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KnockBack")
	UDataTable* KnockBackDataTable;

	/// <summary>
	/// �G�Ǘ��N���X�̃|�C���^
	/// </summary>
	UPROPERTY()
	UEnemyManagerSubsystem* EnemyManager;

	/// <summary>
	/// �Q�[���̐i�s�Ǘ��N���X�̃|�C���^
	/// </summary>
	UPROPERTY()
	UGameProgressSubsystem* GameProgress;

	/// <summary>
	/// �G�̍U���R���|�[�l���g�N���X
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "EnemyAttack")
	UEnemyAttackComponent* EnemyAttackComponent;

	/// <summary>
	/// �G��{�X�e�[�^�X
	/// </summary>
	UPROPERTY(EditAnywhere)
	FEnemyStatus EnemyStatus;

	FVector LanchVelocity;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// �G�X�V���\�b�h
	/// </summary>
	/// <param name="Player">�v���C���[</param>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	virtual void OnUpdate(APawn* Player, float DeltaTime) { return; }

	/// <summary>
	/// ���g�����񂾍ۂ̏���
	/// </summary>
	virtual void OnDeath();

	/// <summary>
	/// �U���\������߂鏈��
	/// </summary>
	virtual void CheckCanAttack();

public:

	// �U���\��
	virtual void SetCanAttack(bool CanAttack) { EnemyStatus.CanAttack = CanAttack; }
	virtual bool CanAttack()const { return EnemyStatus.CanAttack; }

private:

	/// <summary>
	/// ���t���[����Actor�ʒu��v�Z
	/// </summary>
	/// <param name="MoveDir">�ړ�����</param>
	/// <param name="Speed">�ړ����x</param>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	/// <returns>���t���[���̈ʒu</returns>
	FVector CalculateNextActorLocation(const FVector& MoveDir, float Speed, float DeltaTime);

	/// <summary>
	/// �ړ������֕�Ԃ�����]��v�Z 
	/// </summary>
	/// <param name="CurrentRotation">���݂̉�]</param>
	/// <param name="TargetRotation">�������ׂ���]</param>
	/// <param name="RotationInterpSpeed">��]��ԑ��x</param>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	/// <returns>��Ԃ�����]����</returns>
	FRotator CalculateRotationToMoveDirection(const FRotator& CurrentRotation, const FRotator& TargetRotation,float RotationInterpSpeed, float DeltaTime);
};
