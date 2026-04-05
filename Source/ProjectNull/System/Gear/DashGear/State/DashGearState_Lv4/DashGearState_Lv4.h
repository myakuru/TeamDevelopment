#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include "DashGearState_Lv4.generated.h"

/// <summary>
/// ダッシュギア状態レベル4クラス
/// </summary>
UCLASS(EditInlineNew, Blueprintable)
class PROJECTNULL_API UDashGearState_Lv4 final : public UDashGearStateBase
{
	GENERATED_BODY()
	
public:

	UDashGearState_Lv4();

public:
	void Update(float DeltaTime) override;

};
