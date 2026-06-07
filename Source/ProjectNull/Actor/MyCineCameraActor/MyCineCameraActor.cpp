// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCineCameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Camera/CameraShakeSourceComponent.h"
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>
#include <ProjectNull/Component/CameraShakeSourceComponent/MyCameraShakeSourceComponent.h>

AMyCineCameraActor::AMyCineCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;

	CameraShakeSourceComponent = CreateDefaultSubobject<UMyCameraShakeSourceComponent>(TEXT("CameraShakeSourceComponent"));
}

void AMyCineCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCineCameraActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	SetActorRelativeLocation(OpeningCameraRelativeOffset);
	SetActorRelativeRotation(OpeningCameraRelativeOffsetRotation);
}

void AMyCineCameraActor::PlayOpeningCutscene()
{
	APlayerBase* Player = Cast<APlayerBase>(GetWorld()->GetFirstPlayerController()->GetPawn());
	if (Player)
	{
		AttachToActor(Player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	ALevelSequenceActor* OutActor = nullptr;
	FMovieSceneSequencePlaybackSettings Settings;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), OpeningSequence, Settings, OutActor);

	if (SequencePlayer)
	{
		SequencePlayer->OnFinished.AddDynamic(this, &AMyCineCameraActor::OnOpeningCutsceneFinished);
		SetActorTickEnabled(true);
		SequencePlayer->Play();
	}

	TestShake();
}

void AMyCineCameraActor::TestShake()
{
	auto* PC = GetWorld()->GetFirstPlayerController();
	if (PC && PC->PlayerCameraManager && OpeningCameraShake)
	{
		PC->PlayerCameraManager->StartCameraShake(OpeningCameraShake);
	}
}

void AMyCineCameraActor::OnOpeningCutsceneFinished()
{
	SetActorTickEnabled(false);

	if (CameraShakeSourceComponent && OpeningCameraShake)
	{
		CameraShakeSourceComponent->StopAllCameraShakes();
	}

	OnCutsceneFinished.Broadcast();
}
