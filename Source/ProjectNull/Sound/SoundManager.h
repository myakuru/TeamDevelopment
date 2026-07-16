#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "SoundManager.generated.h"

class USuperGameInstance;
class USoundBase;
class UAudioComponent;
class USoundConcurrency;

/**
 * 全ての音を管理するマネージャー
 * ループ再生は音側で設定(Looping)
 */
UCLASS()
class PROJECTNULL_API USoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	
	void Initialize(USuperGameInstance* inGameInstance);
	
	/** 2D再生 
	 * inConcurrencySettings : 同じ音を何個まで同時再生できるかを制御する設定
	 * inOwningActor : Concurrency制御で「このアクターごと」に再生数を制限したい場合に指定
	 * inIsUISound : UIサウンドとして扱うかどうかです。trueにすると、ゲームの一時停止中でも再生できるなど、UI向けの扱いに
	 */
	UFUNCTION()
	void Play2D(USoundBase* inSound,float inVolume = 1.0f,float inPitch = 1.0f,
		float inStart = 0.0f,USoundConcurrency* inConcurrencySettings = nullptr,
		const AActor* inOwningActor = nullptr,bool inIsUISound = false);
	
	/** サウンドのポインターを返す2D再生 
	 * ConcurrencySettings : 同じ音を何個まで同時再生できるかを制御する設定
	 * inPersistAcrossLevelTransition : レベル遷移後も再生を継続するか
	 * inAutoDestroy : 再生終了時にUAudioComponentを自動破棄するか
	 */
	UFUNCTION()
	UAudioComponent* Spawn2D(
		USoundBase* inSound,float inVolume = 1.0f,float inPitch = 1.0f,
		float inStart = 0.0f,USoundConcurrency* inConcurrencySettings = nullptr,
		bool inPersistAcrossLevelTransition = false,bool inAutoDestroy = false);
	
	/** 3D再生 
	 *  AttenuationSettings : 距離による音量変化や減衰の設定です
	 *  ConcurrencySettings : 同時再生数を制御する設定です
	 *  OwningActor : Concurrencyをアクター単位で管理したい場合に指定します
	 */
	UFUNCTION()
	void PlayAtLocation(USoundBase* inSound, const FVector& inLocation,
	FRotator inRotation = FRotator::ZeroRotator,float inVolume = 1.0f,
	float inPitch = 1.0f,float inStart = 0.0f,USoundAttenuation* inAttenuationSettings = nullptr,
	USoundConcurrency* inConcurrencySettings = nullptr,const AActor* inOwningActor = nullptr);
	
private:
	UPROPERTY()
	TObjectPtr<USuperGameInstance> GameInstance;
};
