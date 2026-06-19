
#pragma once

#include "CoreMinimal.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "../../../../System/DataTable/KnockBackData/KnockBackData.h"
#include "ProjectNull\System\Interface\CharacterInterface\CharacterInterface.h"
#include "EnemyDataStruct.h"
#include "../CombatCharacterBase.h"
#include <ProjectNull/System/Interface/DamageableInterface/DamageableInterface.h>
#include "EnemyBase.generated.h"

// 前方宣言
class UCapsuleComponent;
class UStateTreeComponent;
class USkeletalMeshComponent;
class UPrimitiveComponent;
class UEnemyDataAsset;
class AEnemyISMManager;

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
class PROJECTNULL_API AEnemyBase:	public AActor
								,	public ICharacterInterface
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
	 * @brief 移動方向のセット
	 * @param MoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& a_MoveDir) { EnemyStatus.MoveDir = a_MoveDir; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param DistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float a_DistSqr) { EnemyStatus.TargetDistanceSqr = a_DistSqr; }

	/**
	* @brief 生存状態をセット
	*/
	virtual void SetIsAlive(bool a_IsAlive) { EnemyStatus.IsAlive = a_IsAlive; }

	/**
	 * @brief 状態タイプをセット
	 * @param a_State 変更先ステート
	 */
	virtual void SetEnemyState(EEnemyState a_TargetState);

	/**
	 * @brief 外部からステートEnum変更を通知
	 * @param a_TargetState 変更先ステート
	 */
	virtual void NotifyChangedStateEnum(EEnemyState a_TargetState);

	/**
	 * @brief 所持する当たり判定チャンネルのレスポンス設定を変更
	 * @param Channel 変更対象チャンネル(WorldStatic,Pawn,etc..)
	 * @param NewResponse レスポンスタイプ(Block・Overlap・Ignore)
	 */
	virtual void NotifyChangedCollisionResponseToChannel(ECollisionChannel Channel, ECollisionResponse NewResponse);

	//~ End Setter


	//~ Begin Getter

	/** ノックバック時の重さを取得 */
	float GetKnockBackWeight()const { return EnemyStatus.KnockBackWeight; }

	/** 移動スピードを取得 */
	float GetMoveSpeed()const { return EnemyStatus.MoveSpeed; }

	/** 回転補間スピード */
	float GetRotationInterpSpeed()const { return EnemyStatus.RotationInterpSpeed; }

	/** 上ることが可能な斜面の角度 */
	float GetWalkableFloorAngle()const { return EnemyStatus.WalkableFloorAngle; }

	/** 超える事の出来る段差の最大の高さ */
	float GetMaxStepHeight()const { return EnemyStatus.MaxStepHeight; }

	/** カプセルの高さの半径(座標補正に使う) */
	float GetCapsuleHalfHeight()const;

	/** EnemyRuntimeへのアクセス、デリゲートへの登録を行う */
	inline UEnemyRuntimeData* GetEnemyRuntimeData() const
	{
		return EnemyRuntimeData;
	}

	/** エネミーマネージャーのゲッター*/
	UEnemyManagerSubsystem* GetEnemyManagerSubsystem() const
	{
		return EnemyManager;
	}

	TSubclassOf<AEnemyISMManager> GetISMManagerClass() const
	{
		return ISMManagerClass;
	}

	/** @brief 敵攻撃コンポーネントの取得 */
	TObjectPtr<UEnemyAttackComponent> GetEnemyAttackComponent()const
	{
		return EnemyAttackComponent;
	}

	/** ISMInstanceIndexを返す*/
	uint32 GetISMInstanceIndex() const
	{
		return ISMInstanceIndex;
	}

	/** ターゲットとの距離を返す*/
	float GetTargetDistanceSqr()const { return EnemyStatus.TargetDistanceSqr; }

	/** 敵の種類ごとにメッシュを返す*/
	virtual UStaticMesh* GetEnemyMesh() const PURE_VIRTUAL(AEnemyBase::GetEnemyMesh, return nullptr;);

	/** ゲームの進行管理クラスのゲッター */
	UGameProgressSubsystem* GetGameProgressSubsystem() const
	{
		return GameProgress;
	}

	bool GetAliveFlg() { return EnemyStatus.IsAlive; }

	//~ End Getter

	/* Begin Character Interface.*/
	virtual void ApplyDamaged(float InDamaged = 1.f)override;

	/* End Character Interface.*/

protected:

	virtual void BeginPlay() override;

	/**
	 * @brief デリゲートへの登録関数
	 */
	virtual void RegisterDelegates();

	/** 敵（自身）のパラメータを更新する */
	virtual void UpdateParams();

	/** SphereCollisionを取得して使うための関数 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Collision")
	TObjectPtr<UCapsuleComponent> CapsuleComponent;

	/** 敵のStateTree*/
	UPROPERTY(VisibleAnywhere, Category = "StateTree")
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

	/** 敵が吹き飛ばされている状態の処理 */
	virtual void MoveToKnockBack(const FVector& KnockBackDir, float KnockBackPower, float DeltaTime);

	/** DataTable 参照 */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "KnockBack")
	UDataTable* KnockBackDataTable;

	///** 敵管理クラスのポインタ */
	UPROPERTY()
	UEnemyManagerSubsystem* EnemyManager;

	/** ゲームの進行管理クラスのポインタ */
	UPROPERTY()
	UGameProgressSubsystem* GameProgress;

	/** 敵の攻撃コンポーネントクラス */
	UPROPERTY(VisibleAnywhere, Category = "EnemyAttack")
	TObjectPtr<UEnemyAttackComponent> EnemyAttackComponent;

	/** アイテムの設定*/
	//UPROPERTY(EditAnywhere, Category = "Drop")
	//TSubclassOf<AExperiencePickup> ExperiencePickupClass;

	/** 敵のランタイムパラメータ管理クラス */
	UPROPERTY(VisibleAnywhere, Category = "EnemyRuntimeData")
	TObjectPtr<UEnemyRuntimeData> EnemyRuntimeData;

	/** 敵基本ステータス */
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
	virtual void OnDeath(){ SetEnemyState(EEnemyState::Death); }

	/**
	 * @brief 被弾時にステートを切り替える
	 */
	virtual void OnHit(){ SetEnemyState(EEnemyState::Hit); }

	/**
	 * @brief 死亡を確定させる処理(StateTree側から呼ぶ)
	 */
	virtual void FinalizeDeath();

	/// <summary>
	/// 攻撃可能かを決める処理
	/// </summary>
	virtual void CheckCanAttack();

	/** 敵が死んださいにパーティクルを出すだけ*/
	virtual void SpawnDeathEffect();

	/** 敵が死んださいに経験値を出す*/
	virtual void SpawnDeathExperience();

	/** アニメーションの変更*/
	virtual void PlayAnimation(int32 NextAnimIndex, bool bLoop);

protected:

	/**
	 * @brief 坂道範囲内に入った時の通知処理
	 */
	virtual void OnEnterSlope()/*override*/;

	float AnimFinishTime = 0.0f;

public:

	// アニメーションEnum
	EEnemyState GetEnemyState() const { return EnemyStatus.StateTag; }

	/**
	 * @brief * アニメーションのセット
	 */
	void SetAnimSequence(UAnimSequence* InAnimSequence, bool LoopFlg);

public:


	/** ISMのどのインスタンスに対応するかを示すインデックス*/
	int32 ISMInstanceIndex = INDEX_NONE;

protected:

	/**
	 * どのISMManagerに登録するかを指定するためのクラス
	 */
	UPROPERTY(EditDefaultsOnly, Category = "SetupISM")
	TSubclassOf<AEnemyISMManager> ISMManagerClass;

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

	/** データアセットからデータを構造体に移す処理*/
	void SetEnemyStatusData(UEnemyDataAsset* InData);
};
