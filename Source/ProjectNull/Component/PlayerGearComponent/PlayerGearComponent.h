#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerGearComponent.generated.h"

// �M�A�̊��N���X
class UGearBase;

// �v���C���[�̒��Ԋ��N���X
class APlayerBase;


/// <summary>
/// �v���C���[�̑����M�A�R���|�[�l���g�N���X
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerGearComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerGearComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/// <summary>
	/// �M�A�ɂ���Ĉړ����u���b�N����Ă��邩�ǂ����𔻒肷��
	/// </summary>
	/// <returns>�ړ����u���b�N����Ă���ꍇ��true�A�����łȂ��ꍇ��false</returns>
	bool IsMovementBlockedByGear() const;

	void ExecuteGear(int32 GearIndex);

	// �Z�b�^�[
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

	// �Q�b�^�[
	inline const TArray<UGearBase*>& GetPlayerGears() const { return PlayerGears; }
	inline int32 GetCurrentGearLevel() const { return CurrentGearLevel; }

private:

	// �v���C���[�i������j�̃|�C���^
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// �v���C���[�̑����M�A�N���X�̔z��
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UGearBase*> PlayerGears;

	// ���݃M�A�̃��x��
	UPROPERTY(EditAnywhere)
	int32 CurrentGearLevel;
};
