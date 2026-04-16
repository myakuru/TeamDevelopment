#pragma once

#include "CoreMinimal.h"
#include "../AttackBase.h"
#include "AutoAttack.generated.h"


// �G�Ǘ��N���X
class UEnemyManagerSubsystem;

//�@�~�^�̎a���U���N���X
class UFanAttackBase;

// ���V����G�t�F�N�g�N���X
class UFloatingWeaponEffect;


// �����U�����
UENUM(BlueprintType)
enum class EAutoAttackType : uint8
{
	Front,
	Ring,
	Count UMETA(Hidden)
};

/// <summary>
/// �����U���N���X
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UAutoAttack final : public UAttackBase
{
	GENERATED_BODY()

public:

	UAutoAttack();

public:

	void Initialize(AActor* Owner)	override;
	void Execute()					override;
	void Update(float DeltaTime,
				APlayerBase* Player = nullptr,
				UEnemyManagerSubsystem* EnemyManager = nullptr)	override;

private:

	/// <summary>
	/// �����U���̊J�n
	/// </summary>
	void StartAutoAttack();

	/// <summary>
	/// ����2�i�K�i�����O��Ɉړ��j�����a���U���̊J�n
	/// </summary>
	void StartAutoRingAttack();

	/// <summary>
	/// �����U���̋��ʍX�V����
	/// </summary>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	/// <param name="ConeSlashParams">���a���U���̃p�����[�^</param>
	/// <param name="EnemyManager">�G�Ǘ��N���X</param>
	void UpdateAutoAttack(float DeltaTime, UFanAttackBase& RingPulseSlashAttack, class UEnemyManagerSubsystem* EnemyManager);

	
	// �����U���̊Ԋu����
	UPROPERTY(EditAnywhere)
	float AutoAttackInterval;

	// �O����󎩓��U������̎��͍U���x������
	UPROPERTY(EditAnywhere)
	float FrontToRingDelay;

	// �����U���̃p�����[�^�}�b�v�z��
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, UFanAttackBase*> AutoAttackParamsMap;

	// ================================================================
	// �G�t�F�N�g�֘A
	// ================================================================

	// ���V����G�t�F�N�g�N���X�̃}�b�v�z��
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EAutoAttackType, UFloatingWeaponEffect*> FloatingWeaponMap;

	// ================================================================
	// �^�C�}�[�֘A
	// ================================================================

	// �O����󎩓��U���^�C�}�[
	FTimerHandle AutoFrontConeAttackTimerHandle;

	// �O����󎩓��U������̎��͍U���x���^�C�}�[
	FTimerHandle FrontToRingDelayTimerHandle;

};
