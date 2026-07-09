
#pragma once

#include "CoreMinimal.h"
#include "../../CombatCharacterBase.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossDataAsset.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include <ProjectNull/Data/CharacterRuntimeData/EnemyBossRuntimeData/EnemyBossRuntimeData.h>
//#include <ProjectNull/System/Interface/DamageableInterface/DamageableInterface.h>
#include <ProjectNull/System/Interface/CharacterInterface/CharacterInterface.h>
#include "EnemyBossBase.generated.h"

class UPawnSensingComponent;
class UStateTreeComponent;

/* 
* 敵（ボス / 中ボス）の中間基底クラス
* ※雑魚的のEnemyBaseとは別系統にしている
* AIControllerをActor継承だとしようできないため、Character継承を継承したCombatCharacterBaseを継承している
* HPやConfigなどの雑魚的と共有したい部分はインターフェース/コンポーネントに切り出して共通化したい
*/
UCLASS()
class PROJECTNULL_API AEnemyBossBase : public ACombatCharacterBase, public ICharacterInterface
{
	GENERATED_BODY()

public:
	AEnemyBossBase();

protected:
	virtual void BeginPlay() override;

	FVector ActorLocation;

public:	

	// ------------------------------------------------------------------------------------
	// public method
	// ------------------------------------------------------------------------------------
	virtual void Tick(float DeltaTime)			override;						/** 更新*/
	//virtual bool IsAlive()				const	override	{ return false; }	/** 生存確認*/
	virtual void ApplyDamaged(float InDamaged = 1.0f)	override;				/** ダメージを受ける処理*/
	virtual void ApplyKnockBack(const FVector& InOwnerLocation)override {}
	virtual float GetFinalAttackPower()const { return 1.f; }
	virtual void OnHit();
	virtual void RegisterDelegates();											/** デリゲートへの登録関数*/
	void AdvanceHitIndex()			{ EnemyBossRuntimeData->HitIndex++; }		/** 連撃のインデックス増加*/
	void ResetHitIndex()			{ EnemyBossRuntimeData->HitIndex = 0; }		/** 連撃のインデックスを初期化*/
	void RequestFastFallOnNotify()	{ EnemyBossRuntimeData->bShouldFastFallOnNotify = true; }		/** 重力加速フラグをtrueにする関数*/
	void ResetGravity();
	void TryConsumeFastFallRequest();
	void ApplyLocalHitPos(const FVector& HitWorldLocation)override;
	void SpawnDeathEffect();
	/** 視界にPawnが入った時に呼ばれる（PawnSensingのコールバック）*/
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);
	/** ダメージを受けたときに呼ばれ、攻撃者をターゲットにする*/
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamageActor, float Damage,
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/** 攻撃パターン配列を取得 */
	const TArray<FBossAttackPattern>& GetAttackPatterns() const
	{
		static const TArray<FBossAttackPattern> Empty;	// 空のは配列
		if (AttackSet)
		{
			return AttackSet->Patterns;	// DataAssetが割り当ててあれば、その中の配列を返す
		}
		else
		{
			return Empty;				// なければ空配列を返す
		}
	}

	// 活動終了
	void BossFinalize();


	// ------------------------------------------------------------------------------------
	// ゲッター
	// ------------------------------------------------------------------------------------
	/** 次の攻撃アクション状態*/
	const EBossActionType GetCurrentAction()			const	{ return EnemyBossRuntimeData->CurrentAction; }
	const EBossActionType GetActionPriority()			const	{ return EnemyBossRuntimeData->ActionPriority; }	/** Decideがアクションを決める際に優先度が高いものを優先するようにするためのゲッター*/
	const EBossActionType GetPrevAction()				const	{ return PrevAction; }
	int32 GetHitIndex()									const	{ return EnemyBossRuntimeData->HitIndex; }			/** 連撃のインデックスのゲッター*/
	const FBossAttackPattern& GetCurrentAttack()		const	{ return EnemyBossRuntimeData->CurrentAttack; }		/** 現在の攻撃を取得*/
	float GetNearRange()								const	{ return NearRange; }		/** 近距離攻撃範囲のゲッター*/
	float GetStrafeChance()								const	{ return StrafeChance; }	/** 連撃確率のゲッター*/
	float GetWalkSpeed()								const	{ return WalkSpeed; }		/** 歩く速度のゲッター*/
	float GetRunSpeed()									const	{ return RunSpeed; }		/** 走る速度のゲッター*/
	AActor* GetTargetActor()							const	{ return TargetActor; }		/** 現在の追尾対象を取得（Evaluatorが毎フレーム読む） */
	TObjectPtr<UNiagaraComponent> GetBreathNiagara()	const	{ return BreathEffect; }
	UFUNCTION(BlueprintPure, Category = "EnemyBoss")
	bool BossIsAlive() const { return EnemyBossRuntimeData->IsAlive; }

	// ------------------------------------------------------------------------------------
	// セッター
	// ------------------------------------------------------------------------------------
	void SetCurrentAction(EBossActionType InAction)		{ EnemyBossRuntimeData->CurrentAction = InAction; }	/** 次の攻撃をいれる*/
	void SetPrevAction(EBossActionType InAction)		{ PrevAction = InAction; }
	void SetTargetActor(AActor* InTarget)				{ TargetActor = InTarget; }							/** 追尾対象を設定（nullptrでロスト扱い） */
	void SetActionPriority(EBossActionType InAction)	{ EnemyBossRuntimeData->ActionPriority = InAction; }	/** Decideがアクションを決める際に優先度付で使用する*/
	void SetNextAttack(EBossActionType InAttack)		{ EnemyBossRuntimeData->CurrentAttack = AttackSet->Patterns[static_cast<int>(InAttack)]; }
	void SelectNextAttack(EBossActionType InAction)
	{
		for (const FBossAttackPattern& P : GetAttackPatterns())
		{
			if (P.ActionType == InAction)
			{
				EnemyBossRuntimeData->CurrentAttack = P;
				return;
			}
		}
	}

	/** 距離に応じて撃てる技を選ぶ。撃てる技がなければfalseを返す*/
	bool SelectAttackByDistance(float Dist)
	{
		TArray<const FBossAttackPattern*> Cand;		// 候補を入れる箱

		// 全パターンをみて、今の距離で撃てるものだけ候補に入れる
		for (const FBossAttackPattern& P : GetAttackPatterns())
		{
			//const bool bRangeOK = (Dist >= P.MinRange && Dist <= P.MaxRange);	// 距離条件を満たすか
			const bool bAttackTypeOK = (P.ActionType == EnemyBossRuntimeData->CurrentAction);			// 攻撃タイプが今のアクションと同じか
			if (/*bRangeOK && */bAttackTypeOK) 
			{ Cand.Add(&P); }
		}
		// 撃てる技が1つもなければ失敗
		if (Cand.Num() == 0) { return false; }

		// 候補からランダムに一つ選んで、ボスの今の攻撃にセット
		EnemyBossRuntimeData->CurrentAttack = *Cand[FMath::RandRange(0, Cand.Num() - 1)];

		UE_LOG(LogTemp, Warning, TEXT("CurrentAction = %s"),
			*UEnum::GetValueAsString(EnemyBossRuntimeData->CurrentAction));

		UE_LOG(LogTemp, Warning, TEXT("CurrentAttack.ActionType = %s"),
			*UEnum::GetValueAsString(EnemyBossRuntimeData->CurrentAttack.ActionType));

		//HitIndex = 0;
		return true;
	}

protected:

	// ------------------------------------------------------------------------------------
	// protected method
	// ------------------------------------------------------------------------------------
	/** データアセットからデータを構造体に移す処理*/
	void SetEnemyBossStatusData(UEnemyBossDataAsset* InData);

	/// <summary>
	/// 自身が死んだ際の処理
	/// </summary>
	virtual void OnDeath() {  }

	// ------------------------------------------------------------------------------------
	// protected variables
	// ------------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;					/** 視界センサー。視野角・視認距離はコンストラクタで設定 */

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UStateTreeComponent> StateTreeComp;						/** StateTree実行コンポーネント（StateTreeアセットはこの詳細で割り当てる） */

	UPROPERTY(EditAnywhere, Category = "AI")
	EBossActionType PrevAction = EBossActionType::ApproachWalk;			/** 前に行動したときのアクションを保存しておく*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AActor> TargetActor = nullptr;							/** 追尾対象。視認/被弾で設定され、Evaluator経由でStateTree全体へ配布される */

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnemyBossAttackSet> AttackSet = nullptr;				/** このボスの攻撃セット（BPで割り当てる） */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunSpeed = 400.0f;											/** ボスの走るときの移動スピード*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 200.0f;											/** ボスの歩く時の移動スピード*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float NearRange = 300.0f;											/** ボスの近距離攻撃範囲*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float StrafeChance = 0.4f;											/** 様子見確率*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float FastFallGravityScale = 5.0f;								/** 重力の追加加速度フラグ*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FRotator NiagaraRotOffset = { -10.0f,0.0f,0.0f };

	/** 敵基本ステータス */
	UPROPERTY(EditAnywhere)
	FEnemyBossStatus EnemyBossStatus;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UEnemyBossDataAsset> EnemyDataAsset;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<UEnemyBossRuntimeData> EnemyBossRuntimeData;

	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraComponent> BreathEffect;

	UPROPERTY(EditAnywhere, Category = "VFX")
	TObjectPtr<UNiagaraSystem> DeathEffect;


	/** ヒットした個所を光らせる*/
	UPROPERTY()
	TArray<TObjectPtr<UMaterialInstanceDynamic>> DynamicMaterials;
	FTimerHandle HitFlashTimerHandle;
	void EndLocalHitFlash();

	UPROPERTY(EditAnywhere, Category = "HitColor")
	float HitPower = 5.0f;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	float HitRadius = 100.0f;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	FVector HitColor;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	float HitTimeDuration = 0.2f;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	float NoiseScale = 0.1f;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	float NoisePower = 1.5f;
	UPROPERTY(EditAnywhere, Category = "HitColor")
	float HitEmissivePower = 10.0f;

};
