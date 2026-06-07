// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CineCameraActor.h"
#include "MyCineCameraActor.generated.h"

class ULevelSequence;
class ALevelSequenceActor;
class ULevelSequencePlayer;
class UMyCameraShakeSourceComponent;
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

	virtual void Tick(float DeltaTime) override;

	void PlayOpeningCutscene();

	void TestShake();

	UPROPERTY(BlueprintAssignable)
	FOnCutsceneFinished OnCutsceneFinished;

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

	/** カメラシェイクの発生源コンポーネント */
	UPROPERTY(VisibleAnywhere, Category = "Opening")
	TObjectPtr<UMyCameraShakeSourceComponent> CameraShakeSourceComponent;

	/** オープニングで再生するカメラシェイク */
	UPROPERTY(EditAnywhere, Category = "Opening")
	TSubclassOf<UCameraShakeBase> OpeningCameraShake;

	/** シーケンス終了時に呼ばれる */
	UFUNCTION()
	void OnOpeningCutsceneFinished();

	UPROPERTY()
	TObjectPtr<ULevelSequencePlayer> SequencePlayer;
};
