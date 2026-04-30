#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "KnockBackData.generated.h"

/// <summary>
/// DataTable�p�̍\���̂�C++�ō쐬����ꍇ�͐e�N���X��FTableRowBase��w��
/// </summary>
USTRUCT(Blueprintable)
struct PROJECTNULL_API FKnockBackData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchSpeed = 500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LaunchAngleDeg = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Deceleration = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float GravityScale = 1.0f;
};