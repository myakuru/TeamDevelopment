#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "KnockBackData.generated.h"

/// <summary>
/// DataTable用の構造体をC++で作成する場合は親クラスにFTableRowBaseを指定
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