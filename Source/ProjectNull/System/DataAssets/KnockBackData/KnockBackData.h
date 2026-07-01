#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "KnockBackData.generated.h"

/**
 * ノックバックに必要なデータ
 */
UCLASS()
class PROJECTNULL_API UKnockBackData : public UDataAsset
{
	GENERATED_BODY()
	
public:

	/**	ノックバック速度 */
	UPROPERTY(EditAnywhere)
	float KnockBackSpeed = 300.f;

};
