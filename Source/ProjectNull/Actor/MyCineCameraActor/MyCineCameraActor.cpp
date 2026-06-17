// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCineCameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"

AMyCineCameraActor::AMyCineCameraActor(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = false;
}

void AMyCineCameraActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyCineCameraActor::PlayOpeningCutscene()
{
}

void AMyCineCameraActor::OnOpeningCutsceneFinished()
{
	SetActorTickEnabled(false);

	//OnCutsceneFinished.Broadcast();
}
