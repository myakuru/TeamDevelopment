
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponAttackState.generated.h"

/// <summary>
// ���V����̍U����ԃN���X
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponAttackState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()

public:
	UFloatingWeaponAttackState();
public:
	void Update(AActor* OwnerActor,float DeltaTime)override;
private:
	
};
