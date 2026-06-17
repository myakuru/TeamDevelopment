#include "PlayerCutsceneComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "DefaultLevelSequenceInstanceData.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>


UPlayerCutsceneComponent::UPlayerCutsceneComponent():
		OwnerPlayer(nullptr),
		CutsceneSequence(nullptr),
		SequencePlayer(nullptr),
		SequenceActor(nullptr),
		DefaultLevelSequenceInstanceData(nullptr)
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerCutsceneComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerPlayer = Cast<APlayerBase>(GetOwner());
}

void UPlayerCutsceneComponent::PlayCutscene()
{
	if (!CutsceneSequence) { return; }

	FMovieSceneSequencePlaybackSettings Settings;

	// 自動再生を無効化
	Settings.bAutoPlay = false;
	Settings.bHidePlayer = true;
	Settings.bDisableLookAtInput = true;
	Settings.bHideHud = true;
	Settings.bDisableMovementInput = true;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		CutsceneSequence,
		Settings,
		SequenceActor
	);

	if (!SequencePlayer || !SequenceActor) { return; }

	// レベルシーケンスの原点をプレイヤーに設定
	DefaultLevelSequenceInstanceData = NewObject<UDefaultLevelSequenceInstanceData>(SequenceActor);
	DefaultLevelSequenceInstanceData->TransformOriginActor = OwnerPlayer;
	SequenceActor->DefaultInstanceData = DefaultLevelSequenceInstanceData;
	SequenceActor->bOverrideInstanceData = true;

	SequencePlayer->Play();
}

bool UPlayerCutsceneComponent::IsPlaying() const
{
	if (!SequencePlayer) { return false; }
	return SequencePlayer->IsPlaying();
}
