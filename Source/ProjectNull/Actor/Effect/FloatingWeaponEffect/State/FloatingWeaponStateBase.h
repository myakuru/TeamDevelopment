
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "FloatingWeaponStateBase.generated.h"

// ���V����N���X
class UFloatingWeaponEffect;

/// <summary>
// ���V����̏�Ԋ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UFloatingWeaponStateBase : public UObject
{
	GENERATED_BODY()

public:

	UFloatingWeaponStateBase();

public:

	/// <summary>
	/// �X�V
	/// </summary>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	virtual void Update(AActor* OwnerActor, float DeltaTime);

	// �Z�b�^�[
	inline void SetOnwer(UFloatingWeaponEffect* SetOwner) { Owner = SetOwner; }

protected:

	UPROPERTY()
	UFloatingWeaponEffect* Owner;

	UPROPERTY(EditAnywhere)
	FTransform Transform;

	// �G�t�F�N�g�̔��a�I�t�Z�b�g
	UPROPERTY(EditAnywhere)
	float RadiusOffset;

	// �G�t�F�N�g�̉�]�I�t�Z�b�g
	UPROPERTY(EditAnywhere)
	FRotator RotatorOffset;

};
