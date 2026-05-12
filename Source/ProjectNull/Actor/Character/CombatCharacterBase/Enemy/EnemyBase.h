
#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "../../../../System/DataTable/KnockBackData/KnockBackData.h"
#include "EnemyDataStruct.h"
#include "../CombatCharacterBase.h"
#include "EnemyBase.generated.h"

// 前方宣言
class UCapsuleComponent;
class USkeletalMeshComponent;
class UPrimitiveComponent;
class UEnemyDataAsset;

template<typename T>
class TStateMachine;

struct FStateMachineDeleter
{
	void operator()(TStateMachine<AEnemyBase>* Ptr) const;
};

/**
 * @brief パーティクル用構造体
 */
USTRUCT(BlueprintType)
struct FEnemyParticle
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraSystem> DeathEffect;
};

// 敵管理クラス
class UEnemyManagerSubsystem;

// ゲームの進行管理クラス
class UGameProgressSubsystem;

// 敵攻撃コンポーネント
class UEnemyAttackComponent;

/** 落とすアイテム*/
class AExperiencePickup;

/** 敵のランタイムパラメータ管理クラス */
class UEnemyRuntimeData;

/// <summary>
/// 敵の中間基底クラス
/// メモ：Characterクラスを継承しているがコンポーネントが多く、
///	重くなる可能性があるためActorを継承する可能性大
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemyBase : public AActor
{
	GENERATED_BODY()
	
public:

	AEnemyBase();
	~AEnemyBase() override;

public:

	/** Poolから取り出されるときに呼ぶ*/
	virtual void Activate(const FVector& LocalPos, UEnemyDataAsset* InData);

	/** Poolに返却するときに呼ぶ*/
	virtual void Deactivate();
	
	//~ Begin Setter

	/**
	 * @brief 敵（自身）が吹き飛ばされる処理
	 * @param playerLocation プレイヤーの座標
	 * @param AttackPower 攻撃力
	 * @param EnemyWeight 敵の重さ
	 */
	virtual void SetKnockBackData(const FVector& PlayerLocation, float AttackPower, float EnemyWeight);
	
	/**
	 * @brief 敵（自身) がダメージを受ける処理
	 * @param AttackPower 攻撃力
	 */
	virtual void SetTakeDamaged(int32 AttackPower = 1);

	/**
	 * @brief 移動方向のセット
	 * @param MoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& a_MoveDir)	{ EnemyStatus.MoveDir = a_MoveDir; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param DistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float a_DistSqr)	{ EnemyStatus.TargetDistanceSqr = a_DistSqr; }

	/**
	 * @brief ノックバックするかセット
	 * @param a_IsKnockBack trueでノックバック
	 */
	virtual void SetIsKnockBack(bool a_IsKnockBack) { EnemyStatus.KnockBackFlg = a_IsKnockBack; }

	//~ End Setter
	
	//~ Begin Getter

	/** StateMachineへのアクセス、Stateの追加・変更に使う */
	TStateMachine<AEnemyBase>& GetStateMachine();

	/** EnemyRuntimeへのアクセス、デリゲートへの登録を行う */
	inline UEnemyRuntimeData* GetEnemyRuntimeData() const
	{
		return EnemyRuntimeData; 
	}
	
	//~ End Getter

protected:
	
	virtual void BeginPlay() override;

	/**
	 * @brief デリゲートへの登録関数
	 */
	virtual void RegisterDelegates();

	/// <summary>
	/// 敵（自身）のパラメータを更新する
	/// </summary>
	virtual void UpdateParams();


	/// <summary>
	/// SphereCollisionを取得して使うための関数
	/// <summary>
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	UCapsuleComponent* CapsuleCollision;

	/** 敵のモデル*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh")
	TObjectPtr<USkeletalMeshComponent> EnemyMesh;

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
	/// 敵の攻撃コンポーネントクラス
	/// </summary>
	UPROPERTY(VisibleAnywhere, Category = "EnemyAttack")
	UEnemyAttackComponent* EnemyAttackComponent;

	/** アイテムの設定*/
	//UPROPERTY(EditAnywhere, Category = "Drop")
	//TSubclassOf<AExperiencePickup> ExperiencePickupClass;

	/** 敵のランタイムパラメータ管理クラス */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntimeData")
	TObjectPtr<UEnemyRuntimeData> EnemyRuntimeData;

	/// <summary>
	/// 敵基本ステータス
	/// </summary>
	UPROPERTY(EditAnywhere)
	FEnemyStatus EnemyStatus;

	/** 死んだ時のエフェクト（パーティクル）*/
	UPROPERTY(EditAnywhere)
	FEnemyParticle EnemyParticle;

	FVector LanchVelocity;

	/** エネミー固有のデータ*/
	const UEnemyDataAsset* GetEnemyData() const { return EnemyDataAsset; }

	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnemyDataAsset> EnemyDataAsset;

public:	
	virtual void Tick(float DeltaTime) override {}

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) {}

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

	/// <summary>
	/// 攻撃可能かを決める処理
	/// </summary>
	virtual void CheckCanAttack();

public:

	// 攻撃可能か
	virtual void SetCanAttack(bool CanAttack) { EnemyStatus.CanAttack = CanAttack; }
	virtual bool CanAttack()const { return EnemyStatus.CanAttack; }

	// アニメーションEnum
	void SetEnemyState(EEnemyState NewState) { CurrentState = NewState; }
	EEnemyState GetEnemyState() const { return CurrentState; }

	/**
	 * @brief * アニメーションのセット
	 */
	void SetAnimSequence(UAnimSequence* InAnimSequence, bool LoopFlg);

public:

	/** アニメーション*/
	//void PlayAnimationMontage();

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
	/// 移動方向へ補間した回転を計算 
	/// </summary>
	/// <param name="CurrentRotation">現在の回転</param>
	/// <param name="TargetRotation">向かうべき回転</param>
	/// <param name="RotationInterpSpeed">回転補間速度</param>
	/// <param name="DeltaTime">デルタタイム</param>
	/// <returns>補間した回転結果</returns>
	FRotator CalculateRotationToMoveDirection(const FRotator& CurrentRotation, const FRotator& TargetRotation, float RotationInterpSpeed, float DeltaTime);

	TUniquePtr<TStateMachine<AEnemyBase>, FStateMachineDeleter> StateMachine;

	/** Stateのタグ*/
	//UPROPERTY(BlueprintReadOnly, Category = "Enemy")
	//FGamePlayTag CurrentStateTag;

	UPROPERTY(VisibleAnywhere, Category = "State")
	EEnemyState CurrentState = EEnemyState::Spawn;

};
