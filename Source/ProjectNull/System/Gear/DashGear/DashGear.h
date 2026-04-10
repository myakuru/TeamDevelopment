#pragma once

#include "CoreMinimal.h"
#include "../GearBase.h"
#include "DashGear.generated.h"


/// <summary>
/// ダッシュギアクラス
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UDashGear final : public UGearBase
{
	GENERATED_BODY()
	
public:

	UDashGear();

public:

	void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent)	override;
	void Execute(int32 CurrentGearLevel)													override;
	void Update(float DeltaTime)															override;

};
