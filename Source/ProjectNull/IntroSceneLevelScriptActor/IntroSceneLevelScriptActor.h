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

private:

	void BeginPlay() override;

};
