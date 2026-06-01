// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "MyCineCameraActor.generated.h"
class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class APlayerBase;

/**
 * 
 */
UCLASS()
class PROJECTNULL_API AMyCineCameraActor : public ACineCameraActor
{
	GENERATED_BODY()

public:
	void UpdateActorRelativeLocation();

	void PlayOpeningCutscene();

protected:
	/** プレイヤーからのカメラ相対オフセット */
	UPROPERTY(EditAnywhere, Category = "Opening")
	FVector OpeningCameraRelativeOffset = { 0,0,0 };

	/** プレイヤーからのカメラの相対角度 */
	UPROPERTY(EditAnywhere, Category = "Opening")
	FRotator OpeningCameraRelativeOffsetRotation = { 0,0,0 };

	/** オープニングで再生するレベルシーケンス */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TObjectPtr<ULevelSequence> OpeningSequence;

	/** ターゲットを切り替える速度変更 */
	UPROPERTY(EditAnywhere, Category = "Opening")
	float TargetBlendSpeed = 0.0f;

	/** シーケンス終了時に呼ばれる */
	UFUNCTION()
	void OnOpeningCutsceneFinished();

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
