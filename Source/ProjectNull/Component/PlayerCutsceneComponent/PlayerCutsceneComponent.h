#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameplayTagContainer.h"

#include "PlayerCutsceneComponent.generated.h"

class APlayerBase;
class ULevelSequence;
class ULevelSequencePlayer;
class ALevelSequenceActor;
class UDefaultLevelSequenceInstanceData;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCutsceneFinished, FGameplayTag, CutsceneId);

USTRUCT(BlueprintType)
struct FCutsceneEntry
{
	GENERATED_BODY()

	/** 再生するシーケンス */
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TObjectPtr<ULevelSequence> Sequence = nullptr;

	/** 再生設定（カットシーンごとに変えたいものだけ） */
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	bool bHidePlayer = true;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	bool bDisableMovementInput = true;

	UPROPERTY(EditAnywhere, Category = "Cutscene")
	bool bHideHud = true;

	/** プレイヤーメッシュを原点にするか（地形固定のムービー等では false） */
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	bool bUsePlayerAsOrigin = true;
};

/** プレイヤーのカットシーン再生コンポーネント */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerCutsceneComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UPlayerCutsceneComponent();

	/** オーナーのプレイヤー */
	UPROPERTY()
	TObjectPtr<APlayerBase> OwnerPlayer;

	/** シーケンサーの再生位置更新 */
	UPROPERTY()
	TObjectPtr<UDefaultLevelSequenceInstanceData> DefaultLevelSequenceInstanceData;

	UPROPERTY(BlueprintAssignable, Category = "Cutscene")
	FOnCutsceneFinished OnCutsceneFinished;

	UFUNCTION(BlueprintCosmetic, Category = "Cutscene")
	bool PlayCutScene(FGameplayTag CutsceneId);

	UFUNCTION(BlueprintCosmetic, Category = "Cutscene")
	void StopCutScene(FGameplayTag CutsceneId);

	/** 現在カットシーン再生中か */
	bool IsPlaying() const;

protected:
	virtual void BeginPlay() override;

private:

	void HandleSequenceFinished();

	/** カットシーンのIDと再生するシーケンスのマップ */
	UPROPERTY(EditAnywhere, Category = "Cutscene")
	TMap<FGameplayTag, FCutsceneEntry> CutsceneEntries;

	FGameplayTag CurrentCutsceneId;

	UPROPERTY()
	ALevelSequenceActor* SequenceActor;

	/** シーケンスプレイヤー */
	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
