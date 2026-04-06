#pragma once

#include "CoreMinimal.h"
#include "../PlayerBase.h"
#include "Robot.generated.h"

/// <summary>
///　ロボット（ワールド内の自身が操作するキャラクター）のクラス
/// </summary>
UCLASS()
class PROJECTNULL_API ARobot : public APlayerBase
{
	GENERATED_BODY()
	
public:

	ARobot();
};
