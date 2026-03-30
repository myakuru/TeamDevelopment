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

	void Initialize(class APlayerBase* Player, class UPlayerGearComponent* GearComponent) override;
	void Update(float DeltaTime)	override;
	void Execute()					override;

private:

	void ApplyLv1()					override;
	void ApplyLv2()					override;
	void ApplyLv3()					override;
	void ApplyLv4()					override;
	void ResetParams()				override;

	bool FrontDash();

};
