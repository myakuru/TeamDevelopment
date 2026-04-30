#pragma once

#include "CoreMinimal.h"
#include "ItemBase.generated.h"

class UItemManagerSubsystem;

UCLASS()
class PROJECTNULL_API AItemBase : public AActor
{
	GENERATED_BODY()

public:

	AItemBase();

protected:

	virtual void BeginPlay()override {}
	//virtual void EndPlay()override {}

	/** アイテムマネージャーのポインタ*/
	UPROPERTY()
	UItemManagerSubsystem* ItemManager;

public:

	/** 更新*/
	virtual void OnUpdate(APawn* Player, float DeltaTime) {}

};
