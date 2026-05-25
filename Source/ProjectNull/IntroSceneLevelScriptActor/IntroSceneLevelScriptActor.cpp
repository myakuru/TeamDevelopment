// Fill out your copyright notice in the Description page of Project Settings.


#include "IntroSceneLevelScriptActor.h"
#include "Kismet/GameplayStatics.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

void AIntroSceneLevelScriptActor::BeginPlay()
{
	Super::BeginPlay();

	// プレイヤーのカメラを降下シーン用の位置に移動させる
	PlayerIntroSceneStart();
}

void AIntroSceneLevelScriptActor::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AIntroSceneLevelScriptActor::PlayerIntroSceneStart()
{
	APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(this, 0);

	Player = Cast<APlayerBase>(PlayerPawn);

	if (Player)
	{
		SpringArmComponent = Player->GetSpringArmComponent();
		CameraComponent = Player->GetCameraComponent();

		SpringArmComponent->SetRelativeLocation(FVector(-300.0f, 0.0f, 300.0f));
		CameraComponent->AddLocalRotation(FRotator(-180.0f, 0.0f, 0.0f));
	}
}
