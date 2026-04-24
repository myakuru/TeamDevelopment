#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "FanAttackBase.generated.h"

/**
 * ���U���R���|�[�l���g�̊��N���X
 */
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFanAttackBase : public UAttackBase
{
	GENERATED_BODY()
	
public:

	UFanAttackBase();

public:

	/// <summary>
	/// �J�n�̍ۂ̏�����
	/// </summary>
	void Start();
	 
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	/// /// <returns>�X�V����</returns>
	bool UpdateAttack(float DeltaTime);

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool CanDeactivate();

	/// <summary>
	/// 
	/// </summary>
	/// <returns></returns>
	bool IsActiveFirstFrame();

	/// <summary>
	/// 
	/// </summary>
	void UpdatePrevActiveFlg();

	/// <summary>
	/// �U���͈͓�Ƀ^�[�Q�b�g�����邩�ǂ����̔���
	/// </summary>
	/// <param name="Target			">�^�[�Q�b�g	</param>
	/// <param name="OwnerLocation	">�U���҂̈ʒu	</param>
	/// <returns>�^�[�Q�b�g���U���͈͓���ǂ���</returns>
	virtual bool IsTargetInRange(AActor* Target, const FVector& OwnerLocation) override;

	/// <summary>
	/// �U�������̌v�Z
	/// </summary>
	/// <param name="forwardVector">�O������</param>
	/// <returns>�v�Z����</returns>
	virtual FVector CalcAttackDir(const FVector& forwardVector) const override;

	/// <summary>
	/// ���a�̓���擾
	/// </summary>
	/// <returns>���a�̓��</returns>
	inline float GetRadiusSquared() const { return Radius * Radius; }

	/// <summary>
	/// ��p��cos�l
	/// </summary>
	/// <returns>��p��cos�l</returns>
	inline float GetConeCosine() const { return FMath::Cos(FMath::DegreesToRadians(ConeAngle)); }

protected:

	/// <summary>
	/// �G���X�g�ɑ΂���U������
	/// </summary>
	/// <param name="EnemyManager">�G�Ǘ��N���X�̃A�h���X</param>
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) override;

	/// <summary>
	/// �v���C���[�ɑ΂���U������
	/// </summary>
	/// <param name="Player">�v���C���[�N���X�̃A�h���X</param>
	virtual void AttackJudgePlayer(AActor* Player) override;

public:

	// �U���̎������ԁi�b�j
	UPROPERTY(EditAnywhere)
	float Duration;

	// �o�ߎ���
	float ElapsedTime;

	// ��]���邩�ǂ���
	UPROPERTY(EditAnywhere)
	bool bRotate;

	// ��]���x�i�x/�b�j
	UPROPERTY(EditAnywhere)
	float RotationSpeed;

	// �U�����a
	UPROPERTY(EditAnywhere)
	float Radius;

	// ��̍L���i�p�x�j
	UPROPERTY(EditAnywhere)
	float ConeAngle;

	// �U������
	bool bIsActive;

	// �S�t���[���ŃA�N�e�B�u��������
	bool bPrevActive;

	// �J�n�̊p�x
	UPROPERTY(EditAnywhere)
	float StartAngle;

	// ���݂̊p�x
	UPROPERTY(EditAnywhere)
	float CurrentAngle;

	// �m�b�N�o�b�N�̋���
	UPROPERTY(EditAnywhere)
	float KnockbackPower;

};