#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/System/Combat/CombatTypes/CombatTypes.h"
#include "ProjectNull/Data/CharacterRuntimeData/CharacterRuntimeData.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyDataStruct.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyAttackData/EnemyAttackDataAsset.h"
#include "EnemyMidBossRuntimeData.generated.h"

/** 進行ベクトルが変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMoveDirChanged, const FVector&		/*MoveDir*/);

/** ノックバック時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetLocationChanged, const FVector&/*TargetLocation*/);

/** ステートEnumが変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnStateEnumChanged, EEnemyState		/*StateEnum*/);

/** ターゲットとの距離が変更された時にに呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetDistChanged, float				/*DistSqr*/);

/** ダメージを受けたときに呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnDamageRatioChanged, float			/*DamageToMaxHealthRatio*/);

/** 発動したい攻撃が変更された時に呼び出される */
DECLARE_MULTICAST_DELEGATE_OneParam(FOnAttackDataChanged,const FEnemyAttackData&	/*AttackData*/);

/**
 * 中ボスのランタイムな値を管理する
 */
UCLASS()
class PROJECTNULL_API UEnemyMidBossRuntimeData : public UCharacterRuntimeData
{
	GENERATED_BODY()

public:

	UEnemyMidBossRuntimeData();

	//~ Begin Notify Methods
	
	/** 攻撃が終了した瞬間の時間を登録 */
	void NotifyAttackFinishTime();

	//~ End Notify Methods

	//~ Begin Setter
	/**
	 * @brief				ターゲットとの距離計算処理
	 * @param a_TargetPos	ターゲット座標
	 * @param a_OwnerPos	原点座標
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyMidBossPara")
	void CalcDistanceToTarget(const FVector& InTargetPos, const FVector& InOwnerPos);

	/**
	 * @brief 受けたダメージを最大体力に対する割合として算出する
	 * @param InReciveDamage 受けたダメージ量
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyMidBossPara")
	void CalclateDamageToMaxHealthRatio(const float InReciveDamage);

	/**
	 * @brief				ステートタイプを切り替える
	 * @param a_StateEnum 切り替え先のステートへ変更
	 */
	UFUNCTION(BlueprintCallable, Category = "EnemyMidBossPara")
	void ChangedEnemyState(EEnemyState InStateEnum);

	/**
	 * @brief 発動したい攻撃を切り替える
	 * @param InAttackData 発動したい攻撃データ
	 */
	void ChangedAttackData(const FEnemyAttackData& InAttackData);

	/**
	 * @brief ターゲットの座標をセット
	 * @param InTargetLocation ターゲットの座標
	 */
	void SetTargetLocation(const FVector& InTargetLocation);

	/**
	 * @brief 最終的なHP
	 * @param InFinalHP 計算後の最終HP
	 */
	void SetFinalHP(float InFinalHP) { Health.Max = Health.Current = InFinalHP; }

	/**
	 * @brief 基礎攻撃力のセット
	 * @param InBasePower 基礎攻撃力
	 */
	void SetBaseAttackPower(float InBasePower) { Attack.Base = InBasePower; }
	//~ End Setter
	
	
	//~ Begin Getter

	/**
	 * @brief 攻撃インターバル分、時間が経過したか
	 * @param InInterval インターバル(秒)
	 * @return 経過していたらtrue
	 */
	bool HasAttackIntervalElapsed(float InInterval);
	//~ End Getter

public:

	/** 進行方向が変更された時に呼び出される */
	FOnMoveDirChanged		OnMoveDirChanged;

	/**	 */
	FOnTargetLocationChanged OnTargetLocationChanged;

	/** ターゲットとの距離が変更された時に呼び出される */
	FOnTargetDistChanged	OnTargetDistChanged;

	/** ダメージを受けたときに呼び出される */
	FOnDamageRatioChanged	OnDamageRatioChanged;

	/**	ノックバック状態が変更された時に呼び出される */
	FOnStateEnumChanged		OnStateEnumChanged;

	/**	発動したい攻撃が変更された時に呼び出される */
	FOnAttackDataChanged 	OnAttackDataChanged;

private:

	/** 進行ベクトル */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	FVector	MoveDir = FVector::ZeroVector;

	/**	ターゲットの座標 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	FVector TargetLocation = FVector::ZeroVector;

	/** ターゲットとの距離の二乗値 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	float	TargetDistanceSqr = 0.0f;

	/**	最大HPに対して受けたダメージ割合 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	float	DamageToMaxHealthRatio = 0.f;

	/**	攻撃が終了した瞬間の時間(秒) */
	UPROPERTY()
	float AttackFinishTime = 0.f;

	/**	ステートEnum */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	EEnemyState StateEnum = EEnemyState::Idle;
	
	/**	発動する攻撃情報 */
	UPROPERTY(VisibleAnywhere, Category = "EnemyMidBossRuntime")
	FEnemyAttackData AttackData = FEnemyAttackData();
};
