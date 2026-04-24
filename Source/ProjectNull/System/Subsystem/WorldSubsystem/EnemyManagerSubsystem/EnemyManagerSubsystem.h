// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "EnemyManagerSubsystem.generated.h"

// �G�̒��Ԋ��N���X
class AEnemyBase;

/// <summary>
/// �G�Ǘ��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UEnemyManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
	
public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;

	/// <summary>
	/// �G�̍X�V����
	/// </summary>
	/// <param name="deltaTime">�f���^�^�C��</param>
	void UpdateEnemies(float DeltaTime);

	/// <summary>
	/// �G�̃��X�g�֑Ώۂ�o�^
	/// </summary>
	/// <param name="enemy">�G�̃|�C���^</param>
	void RegisterEnemy(AEnemyBase* Enemy);

	/// <summary>
	/// �G�̃��X�g����Ώۂ�폜
	/// </summary>
	/// <param name="enemy">�G�̃|�C���^</param>
	void RemoveEnemy(AEnemyBase* Enemy);

	/// <summary>
	/// ���X�g��̓G��
	/// </summary>
	/// <returns>�G��</returns>
	int32 GetEnemyNum() const { return EnemyGruntList.Num(); }

	/// <summary>
	/// �G��܂Ƃ߂郊�X�g
	/// </summary>
	TArray<AEnemyBase*> GetEnemyList() const { return EnemyGruntList; }	

private:

	/// <summary>
	/// �G��܂Ƃ߂郊�X�g
	/// </summary>
	UPROPERTY()
	TArray<AEnemyBase*> EnemyGruntList;
};
