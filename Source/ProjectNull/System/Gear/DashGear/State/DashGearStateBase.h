#pragma once

#include "CoreMinimal.h"
#include "../../State/GearStateBase.h"
#include "DashGearStateBase.generated.h"

// Niagara�G�t�F�N�g�N���X
class UNiagaraSystem;


/// <summary>
/// �_�b�V���M�A�̏�Ԃ̒��Ԋ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UDashGearStateBase : public UGearStateBase
{
	GENERATED_BODY()

public:	

	UDashGearStateBase();

public:

	virtual void Execute(int32 CurrentGearLevel) override;
	virtual void Update(float DeltaTime) override;

protected:
	
	/// <summary>
	/// �_�b�V������
	/// </summary>
	void Dash();

	/// <summary>
	/// �_�b�V���U���G�t�F�N�g�̍Đ�
	/// </summary>
	void PlayDashEffect();

	// ���i�O���j�����a���U���̃G�t�F�N�g
	UPROPERTY(EditAnywhere, Category = "Effect")
	UNiagaraSystem* DashEffect;

private:

	/// <summary>
	/// �_�b�V���U���̍X�V
	/// </summary>
	void UpdateDashAttack();

	// �_�b�V���U���̋����̓��i��r�p�j
	UPROPERTY(EditAnywhere)
	float DashAttackRangeSquared;

	// �_�b�V���̑��x
	UPROPERTY(EditAnywhere)
	float DashSpeed;

	// �_�b�V���G�t�F�N�g�̎�������
	UPROPERTY(EditAnywhere)
	float DashEffectDuration;
};
