// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

/// <summary>
/// 敵基本ステータス
/// </summary>
USTRUCT(BlueprintType)
struct FEnemyStatus
{
	GENERATED_BODY()

public:
	//　移動方向
	UPROPERTY(EditAnywhere)
	FVector MoveDir = FVector::ZeroVector;

	//　移動速度
	UPROPERTY(EditAnywhere)
	float	MoveSpeed = 300.0f;

	//　回転補間速度
	UPROPERTY(EditAnywhere)
	float	RotationInterpSpeed = 5.0f;
};


//　敵管理クラス
class UEnemyManagerSubsystem;


/// <summary>
/// 敵の中間基底クラス
/// メモ：Characterクラスを継承しているがコンポーネントが多く、
///	重くなる可能性があるためActorを継承する可能性大
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemyBase : public ACharacter
{
	GENERATED_BODY()
	
public:
	// Sets default values for this character's properties
	AEnemyBase();
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// 敵（自身）がプレイヤーへ向かう処理
	/// </summary>
	/// <param name="playerLocation"> プレイヤーの座標</param>
	virtual void MoveToPlayer(const FVector& PlayerLocation, float DeltaTime);

	/// <summary>
	/// 敵管理クラスのポインタ
	/// </summary>
	UPROPERTY()
	UEnemyManagerSubsystem* EnemyManager;

	/// <summary>
	/// 敵基本ステータス
	/// </summary>
	UPROPERTY(EditAnywhere)
	FEnemyStatus EnemyStatus;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// 敵更新メソッド
	/// </summary>
	/// <param name="Player">プレイヤー</param>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void OnUpdate(APawn* Player, float DeltaTime) { return; }

	/// <summary>
	/// 自身が死んだ際の処理
	/// </summary>
	virtual void OnDeath();

private:

	/// <summary>
	/// 次フレームのActor位置を計算
	/// </summary>
	/// <param name="MoveDir">移動方向</param>
	/// <param name="Speed">移動速度</param>
	/// <param name="DeltaTime">デルタタイム</param>
	/// <returns>次フレームの位置</returns>
	FVector CalculateNextActorLocation(const FVector& MoveDir, float Speed, float DeltaTime);

	/// <summary>
	///　移動方向へ補間した回転を計算 
	/// </summary>
	/// <param name="CurrentRotation">現在の回転</param>
	/// <param name="TargetRotation">向かうべき回転</param>
	/// <param name="RotationInterpSpeed">回転補間速度</param>
	/// <param name="DeltaTime">デルタタイム</param>
	/// <returns>補間した回転結果</returns>
	FRotator CalculateRotationToMoveDirection(const FRotator& CurrentRotation, const FRotator& TargetRotation,float RotationInterpSpeed, float DeltaTime);
};
