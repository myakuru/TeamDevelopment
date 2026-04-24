
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatCharacterBase.generated.h"

/// <summary>
/// �q�b�g�|�C���g�̃f�[�^�\��
/// </summary>
USTRUCT(BlueprintType)
struct FHitPointData
{
	GENERATED_BODY()

public:
	
	FHitPointData():
		Max(100.0f),
		Current(100.0f)
	{
	}

	// �ő�HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	float Max;

	// ���݂�HP
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HP")
	float Current;
};

/// <summary>
/// �퓬�L�����N�^�[�̓��v���
/// </summary>
USTRUCT(BlueprintType)
struct FCombatCharacterStatistics
{
	GENERATED_BODY()

public:

	FCombatCharacterStatistics()
	{
	}

	// �q�b�g�|�C���g�̃f�[�^
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HP")
	FHitPointData HP;
};

UCLASS()
class PROJECTNULL_API ACombatCharacterBase : public ACharacter
{
	GENERATED_BODY()

public:
	ACombatCharacterBase();

protected:

	virtual void BeginPlay() override;

	// �퓬�L�����N�^�[�̓��v���
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat")
	FCombatCharacterStatistics CombatStats;

public:	

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	/// <summary>
	/// ���g���_���[�W��󂯂鏈��
	/// </summary>
	/// <param name="Damage">�󂯂�_���[�W��</param>
	virtual void ApplyDamage(float Damage = 1.0f);
	
};
