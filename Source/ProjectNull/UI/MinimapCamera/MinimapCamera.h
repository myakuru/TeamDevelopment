// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MinimapCamera.generated.h"

UCLASS()
class PROJECTNULL_API AMinimapCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinimapCamera();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// 画面をキャプチャするためのコンポーネント
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Minimap")
	class USceneCaptureComponent2D* SceneCaptureComponent;

	// レンダーターターゲット
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Minimap")
	class UTextureRenderTarget2D* MinimapRenderTarget;

	// キャラクターの追跡
	UPROPERTY()
	class ACharacter* TargetCharacter;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
