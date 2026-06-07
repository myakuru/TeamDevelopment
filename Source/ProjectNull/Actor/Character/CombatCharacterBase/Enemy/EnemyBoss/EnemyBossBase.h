
#pragma once

#include "CoreMinimal.h"
#include "../../CombatCharacterBase.h"
#include "EnemyBossDataStruct.h"
#include "EnemyBossDataAsset.h"
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
class PROJECTNULL_API AEnemyBossBase : public ACombatCharacterBase
{
	GENERATED_BODY()

public:
	AEnemyBossBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	
	/*
	* 視界にPawnが入った時に呼ばれる（PawnSensingのコールバック）*
	*/
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

	/** ダメージを受けたときに呼ばれ、攻撃者をターゲットにする*/
	UFUNCTION()
	void HandleTakeAnyDamage(AActor* DamageActor, float Damage,
		const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser);

	/** 現在の追尾対象を取得（Evaluatorが毎フレーム読む） */
	AActor* GetTargetActor() const { return TargetActor; }

	/** 攻撃パターン配列を取得 */
	const TArray<FBossAttackPattern>& GetAttackPatterns() const
	{
		static const TArray<FBossAttackPattern> Empty;
		if (AttackSet)
		{
			return AttackSet->Patterns;
		}
		else
		{
			return Empty;
		}
	}

	/** 現在選択中の攻撃を取得（Approach,Playが読む）*/
	//const FBossAttackPattern& GetAttackPattern()		const		{ return AttackPattern; }
	/** 次の攻撃アクション状態*/
	const EBossActionType GetNextAction()			const		{ return NextAction; }
	void SetNextAction(EBossActionType InAction)				{ NextAction = InAction; }
	int32 GetHitIndex() const		{ return HitIndex; }			/** 連撃のインデックスのゲッター*/
	void AdvanceHitIndex()			{ HitIndex++; }				/** 連撃のインデックス増加*/

	float GetNearRange() const	{ return NearRange; }		/** 近距離攻撃範囲のゲッター*/
	float GetStrafeChance() const { return StrafeChance; }	/** 連撃確率のゲッター*/
	float GetWalkSpeed() const	{ return WalkSpeed; }		/** 歩く速度のゲッター*/
	float GetRunSpeed() const		{ return RunSpeed; }			/** 走る速度のゲッター*/



	const FBossAttackPattern& GetCurrentAttack() const { return CurrentAttack; }

	/** 追尾対象を設定（nullptrでロスト扱い） */
	void SetTargetActor(AActor* InTarget) { TargetActor = InTarget; }

	/** 攻撃パターンをセット*/
	//void SetAttackPattern(const FBossAttackPattern& Pattern) { AttackPattern = Pattern; }

	/** 距離に応じて撃てる技を選ぶ。撃てる技がなければfalseを返す*/
	bool SelectAttackByDistance(float Dist)
	{
		TArray<const FBossAttackPattern*> Cand;
		for (const FBossAttackPattern& P : GetAttackPatterns())
		{
			if (Dist >= P.MinRange && Dist <= P.MaxRange) { Cand.Add(&P); }
		}
		if (Cand.Num() == 0) { return false; }

		CurrentAttack = *Cand[FMath::RandRange(0, Cand.Num() - 1)];
		HitIndex = 0;
		return true;
	}

protected:

	/** 視界センサー。視野角・視認距離はコンストラクタで設定 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;

	/** StateTree実行コンポーネント（StateTreeアセットはこの詳細で割り当てる） */
	UPROPERTY(EditAnywhere, Category = "AI")
	TObjectPtr<UStateTreeComponent> StateTreeComp;

	/** 追尾対象。視認/被弾で設定され、Evaluator経由でStateTree全体へ配布される */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	TObjectPtr<AActor> TargetActor = nullptr;

	///** 攻撃パターンを所持*/
	//UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	//FBossAttackPattern AttackPattern;

	/** 攻撃が何連撃目か*/
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "AI")
	int32 HitIndex = 0;

	/** このボスの攻撃セット（BPで割り当てる） */
	UPROPERTY(EditDefaultsOnly, Category = "AI")
	TObjectPtr<UEnemyBossAttackSet> AttackSet = nullptr;

	/** ボスの走るときの移動スピード*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float RunSpeed = 400.0f;

	/** ボスの歩く時の移動スピード*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float WalkSpeed = 200.0f;

	/** ボスの近距離攻撃範囲*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float NearRange = 300.0f;

	/** 様子見確率*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI")
	float StrafeChance = 0.4f;

	/** Decideが決めた次のアクション*/
	EBossActionType NextAction = EBossActionType::None;

	/** 現在選択中の攻撃と、連撃の何撃目か */
	FBossAttackPattern CurrentAttack;

};
