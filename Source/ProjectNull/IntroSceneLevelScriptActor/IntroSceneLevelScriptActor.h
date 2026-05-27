// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "IntroSceneLevelScriptActor.generated.h"

class APlayerBase;
class USpringArmComponent;
class UCameraComponent;

/**
 * レベル開始時の降下シーンのレベルスクリプトアクター
 */
UCLASS()
class PROJECTNULL_API AIntroSceneLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IntroScene")
	FVector PlayerStartLocation = { 0.0f, 0.0f, 10000.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IntroScene")
	FVector RelativeLocation = { 0.0f, 0.0f, 0.0f };

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "IntroScene")
	FRotator LocalRotation = { 0.0f, 0.0f, 0.0f };

private:

	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	void PlayerIntroSceneStart();

	TObjectPtr<APlayerBase>				Player;
	TObjectPtr<USpringArmComponent>		SpringArmComponent;
	TObjectPtr<UCameraComponent>		CameraComponent;

};
