// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../../../System/DataTable/KnockBackData/KnockBackData.h"
#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyBase.generated.h"

// 前方宣言
class UCapsuleComponent;
class UPrimitiveComponent;

/// <summary>
/// 敵ステータスのスケーリング情報
/// </summary>
USTRUCT(BlueprintType)
struct FStatScaling
{
	GENERATED_BODY()

public:
	//　基礎数値
	UPROPERTY(EditAnywhere)
	int32 Base = 100;

	//　倍率
	UPROPERTY(EditAnywhere)
	float Scale = 1.0f;

	//　倍率増加量
	UPROPERTY(EditAnywhere)
	float ScalePerKill = 0.005f;

	/// <summary>
	/// 基礎数値 * 倍率
	/// </summary>
	/// <returns>最終的な数値を返す</returns>
	int32 GetFinalValue(int32 Count)
	{
		Scale = 1.0f + Count * ScalePerKill;
		return static_cast<int32>(Base * Scale);
	}
};

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

	//　最終的なヒットポイント
	UPROPERTY(EditAnywhere)
	int32	FinalHP = 100;

	//　スケーリング計算用ヒットポイント
	UPROPERTY(EditAnywhere)
	FStatScaling HPScaling;

	//　最終的な攻撃力
	UPROPERTY(EditAnywhere)
	int32	FinalAttack = 1;

	//　スケーリング計算用攻撃力
	UPROPERTY(EditAnywhere)
	FStatScaling AttackScaling;

	// エネミーの重量
	UPROPERTY(EditAnywhere)
	float	KnockBackWeight = 1.0;

	// ノックバック方向
	FVector KNockBackVelocity = FVector::ZeroVector;

	// エネミーが吹き飛び中の判定フラグ
	bool	KnockBackFlg = false;

	//　経験値
	UPROPERTY(EditAnywhere)
	int EXP = 0;

	//　ギアエネルギー
	UPROPERTY(EditAnywhere)
	int GearEnergy = 0;
};

//　敵管理クラス
class UEnemyManagerSubsystem;

//　ゲームの進行管理クラス
class UGameProgressSubsystem;

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
public:

	/// <summary>
	/// 敵（自身）が吹き飛ばされる処理
	/// </summary>
	virtual void SetKnockBackData(const FVector& playerLocation, float AttackPower, float EnemyWeight);

// エビ追加
public:

	/// <summary>
	/// 敵（自身) がダメージを受ける処理
	/// </summary>
	virtual void SetTakeDamaged(int32 AttackPower = 1);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/// <summary>
	/// 敵（自身）がプレイヤーへ向かう処理
	/// </summary>
	/// <param name="playerLocation"> プレイヤーの座標</param>
	virtual void MoveToPlayer(const FVector& PlayerLocation, float DeltaTime);

	/// <summary>
	/// 敵（自身）のパラメータを更新する
	/// </summary>
	virtual void UpdateParams();

	/// <summary>
	/// SphereCollisionを取得して使うための関数
	/// <summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleCollision;

	//// 他のActorと重なった際に呼ばれる関数
	//UFUNCTION()
	//virtual void OnOverlap(
	//	UPrimitiveComponent* OverlappedComponent,	// 自分側のコリジョン
	//	AActor* OtherActor,							// 触れてきたアクタ
	//	UPrimitiveComponent* OtherComp,				// 触れてきた側のコンポーネント
	//	int32 OtherBodyIndex,						// ボディ番号（基本使わない）
	//	bool bFromSweep,							// Sweepかどうか
	//	const FHitResult& SweepResult				// 当たりの詳細判定
	//);

	/// <summary>
	/// 敵が吹き飛ばされている状態の処理
	/// </summary>
	virtual void MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime);

	// DataTable 参照
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KnockBack")
	UDataTable* KnockBackDataTable;

	/// <summary>
	/// 敵管理クラスのポインタ
	/// </summary>
	UPROPERTY()
	UEnemyManagerSubsystem* EnemyManager;

	/// <summary>
	/// ゲームの進行管理クラスのポインタ
	/// </summary>
	UPROPERTY()
	UGameProgressSubsystem* GameProgress;

	/// <summary>
	/// 敵基本ステータス
	/// </summary>
	UPROPERTY(EditAnywhere)
	FEnemyStatus EnemyStatus;

	FVector LanchVelocity;

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
