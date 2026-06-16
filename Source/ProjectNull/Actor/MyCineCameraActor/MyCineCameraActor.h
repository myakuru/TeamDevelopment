// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "MyCineCameraActor.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class UCameraShakeBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnCutsceneFinished);

/**
 *　オープニングカットシーン用のカメラアクター
 */
UCLASS()
class PROJECTNULL_API AMyCineCameraActor : public ACineCameraActor
{
	GENERATED_BODY()

public:
	AMyCineCameraActor(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());

	virtual void BeginPlay() override;

	void PlayOpeningCutscene();

	//UPROPERTY(BlueprintAssignable)
	//FOnCutsceneFinished OnCutsceneFinished;

protected:
	/** オープニングで再生するレベルシーケンス */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TObjectPtr<ULevelSequence> OpeningSequence;

	/** オープニングで再生するカメラシェイク */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TSubclassOf<UCameraShakeBase> OpeningCameraShake;

	/** シーケンス終了時に呼ばれる */
	UFUNCTION()
	void OnOpeningCutsceneFinished();

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
