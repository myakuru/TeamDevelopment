
#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "EnemyGruntBase.generated.h"

/// <summary>
/// �G�i��ʕ�/�G���j�̒��Ԋ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemyGruntBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyGruntBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// �G�X�V���\�b�h
	/// </summary>
	/// <param name="Player">�v���C���[</param>
	/// <param name="DeltaTime">�f���^�^�C��</param>
	virtual void OnUpdate(APawn* Player, float DeltaTime) override;

private:

	//// ����Actor�Əd�Ȃ����ۂɌĂ΂��֐�
	//virtual void OnOverlap(
	//	UPrimitiveComponent* OverlappedComponent,	// �������̃R���W����
	//	AActor* OtherActor,							// �G��Ă����A�N�^
	//	UPrimitiveComponent* OtherComp,				// �G��Ă������̃R���|�[�l���g
	//	int32 OtherBodyIndex,						// �{�f�B�ԍ��i��{�g��Ȃ��j
	//	bool bFromSweep,							// Sweep���ǂ���
	//	const FHitResult& SweepResult				// ������̏ڍה���
	//) override;

};
