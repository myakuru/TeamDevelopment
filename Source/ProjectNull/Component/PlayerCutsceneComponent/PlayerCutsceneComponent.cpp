#include "PlayerCutsceneComponent.h"
#include "LevelSequencePlayer.h"
#include "DefaultLevelSequenceInstanceData.h"
#include "GameplayTagContainer.h"
#include "LevelSequenceActor.h"

#include <ProjectNull/Actor/Character/CombatCharacterBase/Player/PlayerBase.h>

UPlayerCutsceneComponent::UPlayerCutsceneComponent():
		OwnerPlayer(nullptr),
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

bool UPlayerCutsceneComponent::PlayCutScene(FGameplayTag CutsceneId)
{
	if (IsPlaying()) { return false; }

	// カットシーンIDに対応するエントリーを取得
	const FCutsceneEntry* CutsceneEntry = CutsceneEntries.Find(CutsceneId);
	if (!CutsceneEntry || !CutsceneEntry->Sequence) { return false; }

	// シーケンスプレイヤーの作成
	CurrentCutsceneId = CutsceneId;

	FMovieSceneSequencePlaybackSettings Settings;
	Settings.bAutoPlay = false;
	Settings.bHidePlayer = CutsceneEntry->bHidePlayer;
	Settings.bHideHud = CutsceneEntry->bHideHud;
	Settings.bDisableMovementInput = CutsceneEntry->bDisableMovementInput;
	Settings.bDisableLookAtInput = true;

	// シーケンサーの作成
	SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(
		GetWorld(),
		CutsceneEntry->Sequence,
		Settings,
		SequenceActor);

	if (!SequencePlayer || !SequenceActor) { return false; }

	if (CutsceneEntry->bUsePlayerAsOrigin && OwnerPlayer)
	{
		DefaultLevelSequenceInstanceData = NewObject<UDefaultLevelSequenceInstanceData>(SequenceActor);
		DefaultLevelSequenceInstanceData->TransformOrigin = OwnerPlayer->GetMesh()->GetComponentTransform();
		
		SequenceActor->DefaultInstanceData = DefaultLevelSequenceInstanceData;
		SequenceActor->bOverrideInstanceData = true;
	}

	SequencePlayer->OnFinished.AddDynamic(this, &UPlayerCutsceneComponent::HandleSequenceFinished);
	SequencePlayer->Play();
	return true;
}

void UPlayerCutsceneComponent::StopCutScene(FGameplayTag CutsceneId)
{
}

void UPlayerCutsceneComponent::HandleSequenceFinished()
{
	OnCutsceneFinished.Broadcast(CurrentCutsceneId);
	CurrentCutsceneId = FGameplayTag();
}

bool UPlayerCutsceneComponent::IsPlaying() const
{
	if (!SequencePlayer) { return false; }
	return SequencePlayer->IsPlaying();
}
