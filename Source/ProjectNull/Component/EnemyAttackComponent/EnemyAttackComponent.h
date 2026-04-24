#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"

// �G�L�����N���X
class AEnemyBase;

// �U�����N���X
class UAttackBase;

/// <summary>
/// �G�L����(���[���h��ő���L�����ڊ|���Ēǔ�����L�����N�^�[)�̍U���R���|�[�l���g�N���X
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEnemyAttackComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetOwnerEnemy(AEnemyBase* Enemy) { OwnerEnemy = Enemy; }

private:

	// �N���X�̎�����̃A�h���X
	UPROPERTY()
	AEnemyBase* OwnerEnemy;

	// �G�̍U���N���X�̔z�񃊃X�g
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> EnemyAttacks;
};
