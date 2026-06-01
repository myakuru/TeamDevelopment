// Fill out your copyright notice in the Description page of Project Settings.


#include "MyCineCameraActor.h"
#include "LevelSequenceActor.h"
#include "LevelSequencePlayer.h"
#include "Kismet/GameplayStatics.h"
#include "LevelSequenceActor.h"

#include <ProjectNull/System/Controller/RobotController/RobotController.h>
#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

void AMyCineCameraActor::UpdateActorRelativeLocation()
{
	SetActorRelativeLocation(OpeningCameraRelativeOffset);
	SetActorRelativeRotation(OpeningCameraRelativeOffsetRotation);
}

void AMyCineCameraActor::OnOpeningCutsceneFinished()
{
	// 入力を解放してゲーム開始
	if (ARobotController* RC = Cast<ARobotController>(GetWorld()->GetFirstPlayerController()))
	{
		RC->SetCanReceiveInput(true);
	}

	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerBase* Player = PC ? Cast<APlayerBase>(PC->GetPawn()) : nullptr;

	if (PC && Player)
	{
		PC->SetViewTargetWithBlend(Player, TargetBlendSpeed);
	}
}

void AMyCineCameraActor::PlayOpeningCutscene()
{
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	APlayerBase* Player = PC ? Cast<APlayerBase>(PC->GetPawn()) : nullptr;

	if (Player)
	{
		AttachToActor(Player, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	}

	if (PC)
	{
		PC->SetViewTargetWithBlend(this, TargetBlendSpeed);
	}

	// 入力をブロック
	if (ARobotController* RC = Cast<ARobotController>(GetWorld()->GetFirstPlayerController()))
	{
		RC->SetCanReceiveInput(false);
	}

	ALevelSequenceActor* OutActor = nullptr;

	// レベルシーケンスを再生
	FMovieSceneSequencePlaybackSettings Settings;
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer
	(
		GetWorld(), OpeningSequence, Settings, OutActor
	);

	if (SequencePlayer)
	{
		// 再生終了時に OnOpeningCutsceneFinished を呼ぶ
		SequencePlayer->OnFinished.AddDynamic
		(
			this, &AMyCineCameraActor::OnOpeningCutsceneFinished
		);
		SequencePlayer->Play();
	}
}
