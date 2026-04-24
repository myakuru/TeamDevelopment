#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnemySpawnData.generated.h"

UENUM(BlueprintType)
enum class ESpawnPattern : uint8
{
	Single		UMETA(DisplayName = "Single"),			// �P�̂ŏo��
	Circle		UMETA(DisplayName = "Circle"),			// �~�`�ɏo��
	Line		UMETA(DisplayName = "Line"),			// ����ɏo��
	RandomMulti UMETA(DisplayName = "RandomMulti"),		// �����_���ɕ����̏o��
};

/// <summary>
/// Wave���Ƃ̓G�̏o���p�^�[����Ǘ�����DataTable�p�̍\����
/// </summary>
USTRUCT(BlueprintType)
struct FEnemySpawnUnit
{
	GENERATED_BODY()

	// �o������G�N���X
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AActor> EnemyClass = nullptr;

	// �o����
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpawnCount = 1;

	// �ʃp�^�[����g�����ǂ���
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bOverridePattern = false;

	// �ʃp�^�[��
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ESpawnPattern SpawnPattern = ESpawnPattern::Single;
};

/// <summary>
/// Wave���Ƃ̓G�o������Ǘ����� DataTable �p�\����
/// </summary>
USTRUCT(BlueprintType)
struct PROJECTNULL_API FEnemySpawnData : public FTableRowBase
{
	GENERATED_BODY()

	// �o������G�ꗗ
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FEnemySpawnUnit> Enemies;
};