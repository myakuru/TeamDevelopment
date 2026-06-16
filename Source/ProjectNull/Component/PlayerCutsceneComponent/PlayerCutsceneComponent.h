#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerCutsceneComponent.generated.h"

class APlayerBase;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UDefaultLevelSequenceInstanceData;

/** プレイヤーのカットシーン再生コンポーネント */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerCutsceneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerCutsceneComponent();

protected:
	virtual void BeginPlay() override;

public:
	/**
	 * @brief カットシーンを再生する
	 */
	void PlayCutscene();

	/**
	 * @brief カットシーンが再生中か
	 */
	bool IsPlaying() const;

private:
	/** オーナーのプレイヤー */
	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	/** 再生するLevelSequenceアセット（BP側でアサイン） */
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TObjectPtr<ULevelSequence> CutsceneSequence;

	/** シーケンサーの再生位置更新 */
	UPROPERTY()
	TObjectPtr<UDefaultLevelSequenceInstanceData> DefaultLevelSequenceInstanceData;

	UPROPERTY()
	ALevelSequenceActor* SequenceActor;

	/** シーケンスプレイヤー */
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
