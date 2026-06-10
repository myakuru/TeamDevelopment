
#pragma once

#include "CoreMinimal.h"
#include "../../CombatCharacterBase.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossDataAsset.h"
#include <ProjectNull/System/Interface/DamageableInterface/DamageableInterface.h>
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
class PROJECTNULL_API AEnemyBossBase : public ACombatCharacterBase, public IDamageableInterface
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
	virtual void Tick(float DeltaTime)			override;							/** 更新*/
	virtual bool IsAlive()				const	override	{ return false; }		/** 生存確認*/
	void AdvanceHitIndex()									{ HitIndex++; }		/** 連撃のインデックス増加*/
	void ResetHitIndex()										{ HitIndex = 0; }	/** 連撃のインデックスを初期化*/
	virtual void ReceiveDamage(float Damage)				override;	/** ダメージを受ける処理*/
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


	// ------------------------------------------------------------------------------------
	// ゲッター
	// ------------------------------------------------------------------------------------
	/** 次の攻撃アクション状態*/
	const EBossActionType GetNextAction()				const	{ return NextAction; }
	int32 GetHitIndex()													const	{ return HitIndex; }			/** 連撃のインデックスのゲッター*/
	float GetNearRange()												const	{ return NearRange; }		/** 近距離攻撃範囲のゲッター*/
	float GetStrafeChance()											const	{ return StrafeChance; }	/** 連撃確率のゲッター*/
	float GetWalkSpeed()												const	{ return WalkSpeed; }		/** 歩く速度のゲッター*/
	float GetRunSpeed()												const	{ return RunSpeed; }		/** 走る速度のゲッター*/
	const FBossAttackPattern& GetCurrentAttack()		const	{ return CurrentAttack; }	/** 現在の攻撃を取得*/
	AActor* GetTargetActor()											const	{ return TargetActor; }		/** 現在の追尾対象を取得（Evaluatorが毎フレーム読む） */

	// ------------------------------------------------------------------------------------
	// セッター
	// ------------------------------------------------------------------------------------
	void SetNextAction(EBossActionType InAction)		{ NextAction = InAction; }		/** 次の攻撃をいれる*/
	void SetTargetActor(AActor* InTarget)				{ TargetActor = InTarget; }		/** 追尾対象を設定（nullptrでロスト扱い） */

	/** 距離に応じて撃てる技を選ぶ。撃てる技がなければfalseを返す*/
	bool SelectAttackByDistance(float Dist)
	{
		TArray<const FBossAttackPattern*> Cand;		// 候補を入れる箱

		// 全パターンをみて、今の距離で撃てるものだけ候補に入れる
		for (const FBossAttackPattern& P : GetAttackPatterns())
		{
			if (Dist >= P.MinRange && Dist <= P.MaxRange) { Cand.Add(&P); }
		}
		// 撃てる技が1つもなければ失敗
		if (Cand.Num() == 0) { return false; }

		// 候補からランダムに一つ選んで、ボスの今の攻撃にセット
		CurrentAttack = *Cand[FMath::RandRange(0, Cand.Num() - 1)];
		//HitIndex = 0;
		return true;
	}

protected:

	// ------------------------------------------------------------------------------------
	// protected method
	// ------------------------------------------------------------------------------------
	/** データアセットからデータを構造体に移す処理*/
	//void SetEnemyBossStatusData(UEnemyDataAsset* InData);

	// ------------------------------------------------------------------------------------
	// protected variables
	// ------------------------------------------------------------------------------------
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;		/** 視界センサー。視野角・視認距離はコンストラクタで設定 */

	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UStateTreeComponent> StateTreeComp;					/** StateTree実行コンポーネント（StateTreeアセットはこの詳細で割り当てる） */

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AActor> TargetActor = nullptr;										/** 追尾対象。視認/被弾で設定され、Evaluator経由でStateTree全体へ配布される */

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int32 HitIndex = 0;																			/** 攻撃が何連撃目か*/

	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnemyBossAttackSet> AttackSet = nullptr;				/** このボスの攻撃セット（BPで割り当てる） */

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunSpeed = 400.0f;																	/** ボスの走るときの移動スピード*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 200.0f;																/** ボスの歩く時の移動スピード*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float NearRange = 300.0f;																/** ボスの近距離攻撃範囲*/

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float StrafeChance = 0.4f;																/** 様子見確率*/

	EBossActionType NextAction = EBossActionType::None;				/** Decideが決めた次のアクション*/
	FBossAttackPattern CurrentAttack;													/** 現在選択中の攻撃と、連撃の何撃目か */
	
	///** 敵基本ステータス */
	//UPROPERTY(EditAnywhere)
	//FEnemyBossStatus EnemyBossStatus;

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UBossEnemyDataAsset> EnemyDataAsset;

};
