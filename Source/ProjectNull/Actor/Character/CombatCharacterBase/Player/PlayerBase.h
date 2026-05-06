#pragma once

#include "CoreMinimal.h"
#include "../CombatCharacterBase.h"
#include "PlayerBase.generated.h"

class UPlayerHUDWidget;
class USpringArmComponent;
class UCameraComponent;
class UPlayerAttackComponent;
class UPlayerGearComponent;
class UAttackBase;
class UAutoAttack;
class USuperGameInstance;
class UPlayerRuntimeData;

USTRUCT(BlueprintType)
struct FExpSystem
{
	GENERATED_BODY()

public:

	FExpSystem():
		CurrentExp(0)
	{ }

public:

	void AddExp(int32 AddValue)
	{
		CurrentExp += AddValue;
	}

	UPROPERTY(EditAnywhere)
	int32 CurrentExp;
};


/// <summary>
/// �v���C���[�i���[���h��̎��g�����삷��L�����N�^�[�j�̒��Ԋ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API APlayerBase : public ACombatCharacterBase
{
	GENERATED_BODY()
public:

	APlayerBase();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime)													override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void ApplyDamage(float Damage = 1.0f)										override;

	/**
	 * @brief ギアエネルギーを加算する処理
	 * @param Amount 加算量
	 */
	void AddGearEnergy(float Amount);

	/**
	 * @brief 経験値を加算する処理
	 * @param Amount 加算量
	 */
	void AddExperience(float Amount);

	/// <summary>
	/// �ړ�����
	/// </summary>
	/// <param name="InputVector">���͒l�Ɋ�Â��Ẵx�N�g��XY</param>
	void Move(const FVector2d& InputVector);

	int32 GetCurrentGearLevel() const;

	inline UPlayerGearComponent*			GetGearComponent() const		{ return GearComponent; }
	inline TObjectPtr<USuperGameInstance>	GetSuperGameInstance() const	{ return Instance; }
	inline FExpSystem						GetEXPSystem() const			{ return ExpSystem; }


private:

	/// <summary>
	/// �ړ��ł��邩�ǂ����𔻒肷��
	/// </summary>
	/// <returns>�ړ��ł���Ȃ�true,�ړ��ł��Ȃ��Ȃ�false</returns>
	bool CanMove();

	/// <summary>
	/// HP��HUD�ɔ��f������
	/// </summary>
	void UpdateHUDHP();

	// �o���l�֘A�\����
	FExpSystem ExpSystem;

	// �v���C���[HUD�E�B�W�F�b�g�ւ̃|�C���^
	UPROPERTY()
	UPlayerHUDWidget* HUDWidget;

	
	// �J�����X�v�����O�A�[���R���|�[�l���g
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	USpringArmComponent* SpringArmComponent;

	// �J�����R���|�[�l���g
	UPROPERTY(VisibleAnywhere,Category = "Camera")
	UCameraComponent* CameraComponent;

	UPROPERTY(EditAnywhere, Instanced, Category = "Attack")
	TObjectPtr<UAutoAttack> AutoAttack;
	
	// �M�A�R���|�[�l���g
	UPROPERTY(VisibleAnywhere,Category = "Gear")
	UPlayerGearComponent* GearComponent;

	UPROPERTY()
	TObjectPtr<USuperGameInstance> Instance;

	UPROPERTY(EditAnywhere, Instanced, Category = "RuntimeData")
	TObjectPtr<UPlayerRuntimeData> RuntimeData;
};
