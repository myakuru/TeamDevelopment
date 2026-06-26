
#pragma once

#include "CoreMinimal.h"

#include "Components/ActorComponent.h"

#include "GroundAlignmentComponent.generated.h"

class USceneComponent;

/** 地面の法線に合わせてRootComponentの姿勢を更新するコンポーネント */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UGroundAlignmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UGroundAlignmentComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(
		float DeltaTime,
		ELevelTick TickType,
		FActorComponentTickFunction* ThisTickFunction) override;

	/** Getter */
	inline USceneComponent* GetRootComponent() const { return RootComp; }

private:
		
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComp;

};
