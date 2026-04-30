
#pragma once

#include "CoreMinimal.h"
#include "../PlayerBase.h"
#include "Perula.generated.h"

UCLASS()
class PROJECTNULL_API APerula : public APlayerBase
{
	GENERATED_BODY()

public:
	APerula();

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:

	int32 MaxHp			= 100;
	int32 CurrentHp		= 100;

};
