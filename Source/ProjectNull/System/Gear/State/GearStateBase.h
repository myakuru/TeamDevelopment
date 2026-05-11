
#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "GearStateBase.generated.h"

// �v���C���[�̃M�A�R���|�[�l���g�N���X
class UPlayerGearComponent;

// �v���C���[�̒��Ԋ��N���X
class APlayerBase;

// �M�A�̊��N���X
class UGearBase;

/// <summary>
/// �M�A�̏�Ԃ̊��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UGearStateBase : public UObject
{
	GENERATED_BODY()
	
public:

	UGearStateBase();

public:

	virtual void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent,class UGearBase* Gear);

	/// <summary>
	/// �M�A�̎��s
	/// </summary>
	virtual void Execute(int32 CurrentGearLevel) { return; }

	/// <summary>
	/// �M�A��Ԃ̍X�V
	/// </summary>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	virtual void Update(float DeltaTime) { return; }

	// �M�A��Ԃ̏I��
	virtual void End() { return; }

protected:

	// ������̃v���C���[�̃|�C���^
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// ������M�A�R���|�[�l���g�̃|�C���^
	UPROPERTY()
	UPlayerGearComponent* OwnerGearComponent;

	// ������M�A�̃|�C���^
	UPROPERTY()
	UGearBase* OwnerGear;

};
