#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

// �G�Ǘ��N���X
class UEnemyManagerSubsystem;

// �v���C���[���N���X
class APlayerBase;

class USceneComponent;

/// <summary>
/// �U���̊��N���X
/// �v���C���[��G�̍U���R���|�[�l���g�N���X�Ŏg�p�����U���N���X�̊��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:

	UAttackBase();

public:

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Initialize(class AActor* Owner);

	/// <summary>
	/// �U���̎��s
	/// ���������z�֐��ł���A�h���N���X�ŕK����������K�v������
	/// </summary>
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );
	
	/// <summary>
	/// �X�V����
	/// </summary>
	/// <param name="DeltaTime		">�f���^�^�C��				</param>
	/// <param name="Player			">�v���C���[�N���X�̃A�h���X	</param>
	/// <param name="EnemyManager	">�G�Ǘ��N���X�̃A�h���X		</param>
	virtual void Update(float DeltaTime, AActor* Player = nullptr,UEnemyManagerSubsystem* EnemyManager = nullptr) { return; }

	/// <summary>
	/// �U������
	/// </summary>
	/// <param name="Player			">�v���C���[�N���X�̃A�h���X	</param>
	/// <param name="EnemyManager	">�G�Ǘ��N���X�̃A�h���X		</param>
	virtual void AttackJudge(AActor* Player = nullptr, UEnemyManagerSubsystem* EnemyManager = nullptr);


	/// <summary>
	/// �U���͈͓�Ƀ^�[�Q�b�g�����邩�ǂ����̔���
	/// </summary>
	/// <param name="Target			">�^�[�Q�b�g	</param>
	/// <param name="OwnerLocation	">�U���҂̈ʒu	</param>
	/// <param name="AttackDir		">�U������	</param>
	/// <returns>�^�[�Q�b�g���U���͈͓���ǂ���</returns>
	virtual bool IsTargetInRange(AActor* Target) { return false; }

	/// <summary>
	/// ������̑O���x�N�g������U��������v�Z����
	/// </summary>
	/// <param name="forwardVector">�O���x�N�g��</param>
	/// <returns></returns>
	virtual FVector CalcAttackDir(const FVector& forwardVector)const;

	/**
	 * @brief 攻撃実行可能か
	 * @return 攻撃実行可能ならtrue
	 */
	bool CanExecute()	const { return bCanExecute; }

	/**
	 * @brief 攻撃が有効かどうか
	 * @return 有効であればtrue
	 */
	bool IsActive()		const { return bIsActive; }

protected:

	/// <summary>
	/// �v���C���[�ɑ΂���U������
	/// </summary>
	/// <param name="Player">�v���C���[�N���X�̃A�h���X</param>
	virtual void AttackJudgePlayer(AActor* Player) { return; };
	
	/// <summary>
	/// �G���X�g�ɑ΂���U������
	/// </summary>
	/// <param name="EnemyManager">�G�Ǘ��N���X�̃A�h���X</param>
	virtual void AttackJudgeEnemys(UEnemyManagerSubsystem* EnemyManager) { return; };

	/**	オーナー */
	UPROPERTY()
	AActor* OwnerActor;
	
	/** アタッチ用のルート（位置・回転管理） */
	UPROPERTY()
	USceneComponent* RootComponent;

	/**	攻撃可能フラグ */
	bool bCanExecute = true;

	/**	攻撃有効フラグ */
	bool bIsActive = false;

	FTransform Transform;
};
