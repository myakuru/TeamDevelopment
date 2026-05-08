// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MiniMapActor.generated.h"

UCLASS()
class PROJECTNULL_API AMiniMapActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMiniMapActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** 上から見た視点のを映すカメラ */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	UPROPERTY()
	class ACharacter* TargetCharacter;

	/** カメラの高さ */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	float CameraHeight = 0.0f;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
