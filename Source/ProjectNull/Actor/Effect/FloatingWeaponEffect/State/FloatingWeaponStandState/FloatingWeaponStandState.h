
#pragma once

#include "CoreMinimal.h"
#include "../FloatingWeaponStateBase.h"
#include "FloatingWeaponStandState.generated.h"

/// <summary>
// ���V����̑ҋ@��ԃN���X
/// </summary>
UCLASS(Blueprintable, EditInlineNew)
class PROJECTNULL_API UFloatingWeaponStandState final : public UFloatingWeaponStateBase
{
	GENERATED_BODY()
	
public:
	UFloatingWeaponStandState();
public:

	void Update(AActor* OwnerActor,float DeltaTime)override;
private:

	UPROPERTY(EditAnywhere)
	float OffsetDist;
};
