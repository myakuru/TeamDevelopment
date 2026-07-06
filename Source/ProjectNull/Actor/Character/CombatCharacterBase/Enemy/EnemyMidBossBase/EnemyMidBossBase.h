#pragma once

#include "CoreMinimal.h"
#include "ProjectNull/System/Interface/CharacterInterface/CharacterInterface.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/CombatCharacterBase.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyAttackData/EnemyAttackDataAsset.h"
#include "ProjectNull/Actor/Character/CombatCharacterBase/Enemy/EnemyMidBossBase/DataAsset/EnemyMidBossDataAsset.h"
#include "EnemyMidBossBase.generated.h"

/** 前方宣言 */
class UEnemyMidBossRuntimeData;
class UGameProgressSubsystem;
class UStateTreeComponent;
class UEnemyAttackComponent;

/**
 * 中ボスの基底クラス
 * ステートツリーによる思考制御
 * 雑魚敵と違って、移動はMovementComponentを使用
 */
UCLASS()
class PROJECTNULL_API AEnemyMidBossBase : public ACombatCharacterBase
										, public ICharacterInterface
{
	GENERATED_BODY()

public:

	AEnemyMidBossBase();

protected:

	virtual void BeginPlay()override;

public:

	virtual void Tick(float InDeltaTime) override;				/**	Tick処理 */
	virtual void UpdateParams();								/** 敵（自身）のパラメータを更新する */
	virtual void TransitionIdleToWalk();						/** IdleからWalkへ遷移する */
	virtual void CheckCanAttack();								/** 攻撃可能かの判定をする */
	virtual void FinalizeAttackEnd();							/** 攻撃終了時の最終処理 */
	virtual void FinalizeDeath();								/** 死亡時の最終処理 */
	virtual void ActivateAttack(EEnemyAttackType InAttackTyp);	/** 攻撃の発動処理 */

	/**
	 * @brief 今、攻撃可能距離内にいるかを返す
	 * @return 範囲内ならtrue
	 */
	virtual bool IsInAttackDistance();
	
	/**
	 * @brief 今、追跡可能距離内にいるかを返す
	 * @return 範囲内ならtrue
	 */
	virtual bool IsInChaseDistance();

	/** デリゲートへの登録関数*/
	virtual void RegisterDelegates();

	/** 自身が死んだ際の処理 */
	virtual void OnDeath() { NotifyChangedStateEnum(EEnemyState::Death); }

	/** 被弾時にステートを切り替える */
	virtual void OnHit() { NotifyChangedStateEnum(EEnemyState::Hit); }


	//~ Begin Notify Methods
	/**
	 * @brief 外部からステートEnum変更を通知
	 * @param InTargetState 変更先ステート
	 */
	virtual void NotifyChangedStateEnum(EEnemyState InTargetState);

	/**
	 * @brief 外部からの発動したい攻撃データ変更を通知
	 * @param InAttackData 発動したい攻撃データ
	 */
	virtual void NotifyChangedAttackData(const FEnemyAttackData& InAttackData);

	/**
	 * @brief 所持する当たり判定チャンネルのレスポンス設定を変更
	 * @param InChannel 変更対象チャンネル(WorldStatic,Pawn,etc..)
	 * @param InNewResponse レスポンスタイプ(Block・Overlap・Ignore)
	 */
	virtual void NotifyChangedCollisionResponseToChannel(ECollisionChannel InChannel, ECollisionResponse InNewResponse);

	//~ End Notify Methods


	/* Begin Getter Methods.*/
	/** ノックバック時の重さを取得 */
	float GetKnockBackWeight()const { return EnemyStatus.KnockBackWeight; }

	/** 移動スピードを取得 */
	float GetMoveSpeed()const { return EnemyStatus.MoveSpeed; }

	/** 回転補間スピード */
	float GetRotationInterpSpeed()const { return EnemyStatus.RotationInterpSpeed; }

	/** 攻撃のインターバルを取得 */
	float GetAttackInterval()const { return EnemyStatus.AttackInterval; }

	/**	攻撃可能距離を取得 */
	float GetAttackDistance()const { return EnemyStatus.AttackDistance; }

	/** 中ボスのランタイムデータ管理クラスを取得 */
	virtual const TObjectPtr<UEnemyMidBossRuntimeData>& GetEnemyMidBossRuntimeData()const
	{
		return EnemyMidBossRuntimeData; 
	}
	/* End Getter Methods.*/


	/* Begin Character Interface.*/
	/**	最終的な攻撃力を取得 */
	virtual float GetFinalAttackPower()const override;

	/**
	 * @brief ダメージを受ける処理
	 * @param InDamaged ダメージ量
	 */
	virtual void ApplyDamaged(float InDamaged)override;

	/**
	 * @brief ノックバックを受ける処理
	 * @param InOwnerLocation 攻撃者の位置
	 */
	virtual void ApplyKnockBack(const FVector& InOwnerLocation)override;
	/* End Character Interface.*/

protected:

	//~ Begin Setter
	/**
	 * @brief 移動方向のセット
	 * @param InMoveDir 移動方向
	 */
	virtual void SetMoveDir(const FVector& InMoveDir) { EnemyStatus.MoveDir = InMoveDir; }

	/**
	 * @brief ターゲットとの距離の二乗値セット
	 * @param InDistSqr 距離の二乗値
	 */
	virtual void SetTargetDistanceSqr(float InDistSqr) { EnemyStatus.TargetDistanceSqr = InDistSqr; }

	/**
	 * @brief 状態タイプをセット
	 * @param InTargetState 変更先ステート
	 */
	virtual void SetEnemyState(EEnemyState InTargetState) { EnemyStatus.StateTag = InTargetState; }

	/**
	 * @brief 発動する攻撃データのセット
	 * @param InAttackData 発動する攻撃データ
	 */
	virtual void SetAttackData(const FEnemyAttackData& InAttackData) { AttackData = InAttackData; }
	
	/**
	 * @brief データアセットからパラメータ取得
	 * @param InData データアセット
	 */
	virtual void SetEnemyStatusData(UEnemyDataAsset* InData);
	//~ End Setter


private:

	/** ゲームの進行管理クラスのポインタ */
	UPROPERTY()
	TObjectPtr<UGameProgressSubsystem> GameProgress;

	/**	中ボスのランタイムデータクラス */
	UPROPERTY()
	TObjectPtr<UEnemyMidBossRuntimeData> EnemyMidBossRuntimeData;

	/**	ステートツリーコンポーネント */
	UPROPERTY(EditAnywhere, Category = "MidBoss")
	TObjectPtr<UStateTreeComponent> StateTreeComponent;

	/**	敵の攻撃コンポーネント */
	UPROPERTY(EditAnywhere, Category = "MidBoss")
	TObjectPtr<UEnemyAttackComponent> AttackComponent;

	/**	基本ステータスにセットするパラメータ群データアセット */
	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnemyDataAsset> EnemyDataAsset;

	/**	敵の基本ステータス */
	UPROPERTY()
	FEnemyStatus EnemyStatus;

	/** 使用する攻撃情報 */
	UPROPERTY()
	FEnemyAttackData AttackData;
};
