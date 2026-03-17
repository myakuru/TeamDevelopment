// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameMode.generated.h"

/// <summary>
/// 生き残りゲームモードクラス
/// </summary>
UCLASS()
class PROJECTNULL_API ASurvivalGameMode : public AGameModeBase
{
	GENERATED_BODY()
	
public:

	ASurvivalGameMode();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
