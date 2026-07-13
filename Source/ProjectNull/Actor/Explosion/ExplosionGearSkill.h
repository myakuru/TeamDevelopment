// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ExplosionGearSkill.generated.h"

/** Niagaraエフェクトクラス */
class UNiagaraSystem;

/** スフィアコリジョンコンポーネント */
class USphereComponent;

USTRUCT(BlueprintType)
struct FExplosionData {

	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float Damage;

	UPROPERTY(EditAnywhere)
	float Scale = 1.0f;

	UPROPERTY(EditAnywhere)
	float Delay;

	UPROPERTY(EditAnywhere)
	float IgnitionDelay;

};

UCLASS()
class PROJECTNULL_API AExplosionGearSkill : public AActor
{
	GENERATED_BODY()
	
public:	
	AExplosionGearSkill();

protected:
	virtual void BeginPlay() override;

public:	

	void Initialize(const FExplosionData& InData);

	UFUNCTION(BlueprintCallable)
	void StartExplosionSequence();

private:

	void ApplyData();

	void Explode();

	/** スフィアコリジョンコンポーネント */
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Collision;

	/** 爆発エフェクト情報 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> ExplosionFX;

	/** 爆発前エフェクト情報 */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UNiagaraSystem> PreExplosionFX;

	/** 爆発パラメータデータ */
	UPROPERTY(EditAnywhere)
	FExplosionData Data;

	/** カメラシェイククラス */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> ExplosionCameraShakeClass;

	/** エフェクトと合わせた当たり判定サイズ */
	UPROPERTY(EditAnywhere)
	float CollisionRadius = 0.0f;

	/** タイマーハンドル */
	FTimerHandle ExplosionTimerHandle;
	FTimerHandle PreExplosionTimerHandle;

};
