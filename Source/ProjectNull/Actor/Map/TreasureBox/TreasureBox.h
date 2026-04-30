#pragma once

#include "CoreMinimal.h"
#include "../MapActorBase.h"
#include "TreasureBox.generated.h"

/**
 * プレイヤーが近づくと破壊される宝箱
 */

/**宝箱が落とすアイテムの種類*/
UENUM(BlueprintType)
enum class EDropItemsType : uint8
{
	EHeal	UMETA(DisplayName = "回復アイテム"),
	EExp	UMETA(DisplayName = "経験値"),
	EEnergy	UMETA(DisplayName = "エネルギー"),
	EGear	UMETA(DisplayName = "ギア")
};

UCLASS()
class PROJECTNULL_API ATreasureBox : public AMapActorBase
{
	GENERATED_BODY()

public:
	ATreasureBox();
public:

	void Tick(float DeltaTime) override;
};
