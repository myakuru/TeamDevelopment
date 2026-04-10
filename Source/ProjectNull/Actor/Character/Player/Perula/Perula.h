// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "ProjectNull/Actor/Character/Player/PlayerBase.h"

#include "Perula.generated.h"

UCLASS()
class PROJECTNULL_API APerula : public APlayerBase
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APerula();

	UPROPERTY()
	class UPlayerHUDWidget* PlayerHUDWidget;

	virtual void Tick(float DeltaTime) override;

	virtual void BeginPlay() override;

private:

	int32 MaxHp			= 100;
	int32 CurrentHp		= 100;

};
