
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponEffect.generated.h"


UENUM(BlueprintType)
enum class EFloatingWeaponState : uint8
{
	Stand,
	Attack,
	Count UMETA(Hidden)
};

// Niagara�G�t�F�N�g�N���X
class UNiagaraSystem;

// Niagara�R���|�[�l���g�N���X
class UNiagaraComponent;

// ���a���U���N���X
class UFanAttackBase;

// ���V����̏�Ԋ��N���X
class UFloatingWeaponStateBase;


/// <summary>
/// ���V����G�t�F�N�g�N���X
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponEffect : public UObject
{
	GENERATED_BODY()

public:
	UFloatingWeaponEffect();
public:

	void Initialize();

	/// <summary>
	/// �G�t�F�N�g�Đ��J�n
	/// </summary>
	void Start(USceneComponent* RootComponent);

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="OwnerActor">������̃N���X</param>
	void Update(AActor* OwnerActor,float DeltaTime);

	/// <summary>
	/// ��Ԃ̑J��
	/// </summary>
	/// <param name="State">�X�e�[�g���</param>
	void ChangeState(EFloatingWeaponState State);


	// �Z�b�^�[
	inline void SetOwnerAttack(UFanAttackBase* Owner) { OwnerAttack = Owner; }
	inline void SetTransform(const FTransform& SetTransform) { Transform = SetTransform; }

	// �Q�b�^�[
	inline UFanAttackBase* GetOwnerAttack() const { return OwnerAttack; }

private:

	/// <summary>
	/// Transform�̍X�V
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// �G�t�F�N�g�����ɂ���
	/// </summary>
	void Deactivate();

	/// <summary>
	/// �o���\���ǂ���
	/// </summary>
	/// <returns>�\�Ȃ�true �\����Ȃ��Ȃ�false</returns>
	inline bool CanSpawn() const { return EffectSystem && !EffectComponent; }



	// ��Ƃ���U���N���X
	UPROPERTY()
	UFanAttackBase* OwnerAttack;

	// Niagara�V�X�e��
	UPROPERTY(EditAnywhere)
	UNiagaraSystem* EffectSystem;

	// Niagara�R���|�[�l���g
	UPROPERTY()
	UNiagaraComponent* EffectComponent;

	// �G�t�F�N�g��Transform
	UPROPERTY(EditAnywhere)
	FTransform Transform;

	// �G�t�F�N�g�̔��a�I�t�Z�b�g
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	// �G�t�F�N�g�̉�]�I�t�Z�b�g
	UPROPERTY(EditAnywhere)
	FRotator RotatorOffset;

	// ���V����̏�Ԃ̔z��
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EFloatingWeaponState,UFloatingWeaponStateBase*> States;

	// ���݂̕��V������
	UPROPERTY()
	UFloatingWeaponStateBase* CurrentState;

	UPROPERTY(EditAnywhere)
	FVector StandLocation;
};
