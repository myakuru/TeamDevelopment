#pragma once

#include "CoreMinimal.h"
#include "../DashGearStateBase.h"
#include "DashGearState_Lv4.generated.h"

/// <summary>
/// �_�b�V���M�A��ԃ��x��4�N���X
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
