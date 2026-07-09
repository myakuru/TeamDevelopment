#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WarningShapeActor.generated.h"

/** 警告表示用形状アクター */
UCLASS()
class PROJECTNULL_API AWarningShapeActor : public AActor
{
	GENERATED_BODY()

public:
	
	// Sets default values for this actor's properties
	AWarningShapeActor();

protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
private:
	
	/**
	 * @brief 補間時間
	 */
	UPROPERTY()
	float Duration = 0.f;

	/**
	 * @brief 経過時間
	 */
	UPROPERTY()
	float DurationTimer = 0.f;
};
