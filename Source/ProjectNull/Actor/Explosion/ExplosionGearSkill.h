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
	float Radius;

	UPROPERTY(EditAnywhere)
	float Delay;

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

private:

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
	UPROPERTY()
	FExplosionData Data;

	FTimerHandle TimerHandle;

};
