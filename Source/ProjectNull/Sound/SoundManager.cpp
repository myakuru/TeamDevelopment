#include "SoundManager.h"
#include "Kismet/GameplayStatics.h"

#include <ProjectNull/GameInstance/SuperGameInstance.h>

void USoundManager::Initialize(USuperGameInstance* inGameInstance)
{
	GameInstance = inGameInstance;
}

void USoundManager::Play2D(USoundBase* inSound, float inVolume, float inPitch, float inStart,
	USoundConcurrency* inConcurrencySettings, const AActor* inOwningActor, bool inIsUISound)
{
	if (!inSound)return;
	UGameplayStatics::PlaySound2D(
		GameInstance, inSound,inVolume,
		inPitch,inStart,inConcurrencySettings,
		inOwningActor,inIsUISound);
}

UAudioComponent* USoundManager::Spawn2D(USoundBase* inSound, float inVolume, float inPitch, float inStart,
	USoundConcurrency* inConcurrencySettings, bool inPersistAcrossLevelTransition, bool inAutoDestroy)
{
	if (!inSound)return nullptr;
	return UGameplayStatics::SpawnSound2D(
		GameInstance,inSound,inVolume,
		inPitch,inStart,inConcurrencySettings,
		inPersistAcrossLevelTransition,inAutoDestroy);
}

void USoundManager::PlayAtLocation(USoundBase* inSound, const FVector& inLocation, FRotator inRotation, float inVolume,
	float inPitch, float inStart, USoundAttenuation* inAttenuationSettings, USoundConcurrency* inConcurrencySettings,
	const AActor* inOwningActor)
{
	if (!inSound)return;
	UGameplayStatics::PlaySoundAtLocation(
		GameInstance, inSound, inLocation, 
		inRotation, inVolume,inPitch,inStart,
		inAttenuationSettings,inConcurrencySettings,
		inOwningActor);
}
