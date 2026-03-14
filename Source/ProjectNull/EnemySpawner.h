// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

UCLASS()
class PROJECTNULL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

	/// <summary>
	/// 敵出現制御処理
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "EnemySpawner")
	void SpawnEnemy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> EnemyClass;

	/// <summary>
	/// 最大出現半径
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnMaxRadius = 1000.0f;

	/// <summary>
	/// 最小出現半径
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnMinRadius = 100.0f;

	/// <summary>
	/// 出現間隔（時間）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 2.0f;

	/// <summary>
	/// 敵を一定時間ごとにスポーンするタイマーID
	/// </summary>
	FTimerHandle SpawnTimerHandle;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
