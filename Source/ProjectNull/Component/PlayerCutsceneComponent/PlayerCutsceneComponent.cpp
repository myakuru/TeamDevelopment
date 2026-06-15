#include "PlayerCutsceneComponent.h"

#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "DefaultLevelSequenceInstanceData.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>


UPlayerCutsceneComponent::UPlayerCutsceneComponent():
		OwnerPlayer(nullptr),
		CutsceneSequence(nullptr),
		SequencePlayer(nullptr)
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

	ALevelSequenceActor* SequenceActor = nullptr;

	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;

	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		CutsceneSequence,
		Settings,
		SequenceActor
	);

	if (!SequencePlayer || !SequenceActor) { return; }

	// レベルシーケンスの原点をプレイヤーに設定
	UDefaultLevelSequenceInstanceData* InstanceData = NewObject<UDefaultLevelSequenceInstanceData>(SequenceActor);
	InstanceData->TransformOriginActor = OwnerPlayer;
	SequenceActor->DefaultInstanceData = InstanceData;
	SequenceActor->bOverrideInstanceData = true;

	SequencePlayer->Play();
}

bool UPlayerCutsceneComponent::IsPlaying() const
{
	if (!SequencePlayer) { return false; }
	return SequencePlayer->IsPlaying();
}
