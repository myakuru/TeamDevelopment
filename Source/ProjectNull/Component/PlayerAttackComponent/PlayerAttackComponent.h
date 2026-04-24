#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"


// �v���C���[�N���X
class APlayerBase;

// �U�����N���X
class UAttackBase;

/// <summary>
/// �v���C���[�i���[���h��̎��g�����삷��L�����N�^�[�j�̍U���R���|�[�l���g�N���X
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// �Z�b�^�[
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

private:

	/// <summary>
	/// �U���N���X��쐬���A���X�g�֒ǉ�
	/// </summary>
	/// <typeparam name="AttackType">�U���N���X�̎��</typeparam>
	/// <returns>�쐬�����U���N���X</returns>
	template<typename AttackType>
	AttackType* AddAttack();

	/// <summary>
	/// ���X�g����U���N���X��������Ď擾
	/// </summary>
	/// <typeparam name="AttackType">�U���N���X�̎��</typeparam>
	/// <returns>�������Ď擾�����U���N���X</returns>
	template<typename AttackType>
	AttackType* FindAttack();


	// �v���C���[�i������j�̃|�C���^
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// �v���C���[�̍U���N���X�̔z��
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> PlayerAttacks;

};
