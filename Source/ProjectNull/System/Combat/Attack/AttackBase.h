#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

class USceneComponent;

/**
 * @brief 攻撃の基底クラス
 */
UCLASS(Blueprintable,EditInlineNew)
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()
public:
	UAttackBase();
public:
	/**
	 * @brief 初期化
	 * @param Owner オーナーアクター
	 */
	virtual void Initialize(const TObjectPtr<AActor>& Owner);

	/**
	 * @brief 有効化処理
	 * @param InTargetLocation	ターゲットの座標 
	 */
	virtual void Execute(const FVector& InTargetLocation = FVector::ZeroVector) PURE_VIRTUAL(UAttackBase::Execute, );
	
	/**
	 * @brief 中止処理
	 */
	virtual void Cancel() PURE_VIRTUAL(UAttackBase::Cancel, );

	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float DeltaTime) { return; }

	/**
	 * @brief 当たり判定処理
	 */
	virtual void AttackJudge() { return; }

	/**
	 * @brief 扇範囲内にターゲットがいるかどうかの判定
	 * @param Target HITした対象
	 * @return ターゲットが範囲内にいる場合はtrue
	 */
	virtual bool IsTargetInRange(AActor* Target) { return false; }


	/**
	 * @brief 攻撃方向を計算する
	 * @param ForwardVector 基準となる前方ベクトル(オーナーの前方ベクトル)
	 * @return 計算された攻撃方向ベクトル
	 */
	virtual FVector CalcAttackDir(const FVector& ForwardVector)const;
	
	/**
	 * @brief 攻撃がHITした敵をリストへ追加
	 * @param InActor 攻撃がHITした敵のアドレス
	 */
	void AddHitActors(const TObjectPtr<AActor>& InActor)
	{
		if (!InActor) { return; }
		HitActors.Add(InActor);
	}

	/**
	 * @brief 攻撃がHITした敵リストからの削除
	 * @param InActor 削除対象の敵のアドレス
	 */
	void RemoveActor(const TObjectPtr<AActor>& InActor)
	{
		if (!InActor || !HitActors.Contains(InActor)) { return; }
		HitActors.Remove(InActor);
	}


	/* Begin ProtectedMember Setters */
	/**
	 * @brief オーナークラスのセット
	 * @param InOwnerActor 変更先のオーナーアドレス
	 */
	void SetOwnerActor(const TObjectPtr<AActor>& InOwnerActor)
	{ OwnerActor = InOwnerActor; }

	/**
	 * @brief ルートのセット
	 * @param InRootComp 変更先のルートアドレス
	 */
	void SetRootComponent(const TObjectPtr<USceneComponent>& InRootComp)
	{ RootComponent = InRootComp; }

	/**
	 * @brief 攻撃実行可能不可の切り替え
	 * @param bInCanExecute trueで攻撃可能
	 */
	void SetCanExecute(const bool bInCanExecute) { bCanExecute = bInCanExecute; }

	/**
	 * @brief 攻撃処理の有効無効の切り替え
	 * @param InIsActive trueで有効
	 */
	void SetIsActive(const bool InIsActive) { bIsActive = InIsActive; }

	/** 補正値トランスフォームをセット */
	virtual void SetOffestTransform(const FTransform& InOffsetTransform)
	{
		OffsetTransform = InOffsetTransform;
	}
	/* End ProtectedMember Setters */


	/* Begin ProtectedMember Getters */
	/** クラスのオーナーを取得 */
	TWeakObjectPtr<AActor> GetOwnerActor()const { return OwnerActor; }

	/** ルートを取得 */
	inline USceneComponent* GetRootComponent()const { return RootComponent; }

	/** 攻撃を当てたアクターリストを取得 */
	TSet<TWeakObjectPtr<AActor>> GetHitActors()const { return HitActors; }

	/** オフセットトランスフォームを取得 */
	FTransform GetOffsetTransform()const { return OffsetTransform; }

	/**	最終的なダメージ量を取得 */
	float GetFinalDamage()const { return FinalDamage; }

	/** 攻撃開始までの待ち時間 */
	float GetAttackStartDelay()const { return AttackStartDelay; }

	/**
	 * @brief 攻撃実行可能か
	 * @return 攻撃実行可能ならtrue
	 */
	bool CanExecute()	const { return bCanExecute; }

	/**
	 * @brief 攻撃が有効かどうか
	 * @return 有効であればtrue
	 */
	bool IsActive()		const { return bIsActive; }
	/* End ProtectedMember Getters */

	inline bool AbsoluteRotation() const { return bAbsoluteRotation;}


private:

	/**	オーナー */
	UPROPERTY()
	TWeakObjectPtr<AActor> OwnerActor;
	
	/** アタッチ用のルート（位置・回転管理） */
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComponent;

	/** HITしたアクターリスト */
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActors;

	/** 攻撃時の位置・回転・スケールのオフセット値 */
	UPROPERTY(EditAnywhere)
	FTransform OffsetTransform = FTransform();

	/**	攻撃本来の威力 */
	UPROPERTY(EditAnywhere)
	float AttackPower = 1.f;

	/** 最終的なダメージ量 */
	/** 「威力 + 最終的なオーナーの攻撃力」でダメージを決定 */
	UPROPERTY()
	float FinalDamage = 0.f;

	/**
	* @brief 攻撃開始までの待ち時間
	*/
	UPROPERTY(EditAnywhere)
	float AttackStartDelay = 0.f;

	/**	攻撃可能フラグ */
	UPROPERTY()
	bool bCanExecute = false;

	/**	攻撃有効フラグ */
	UPROPERTY()
	bool bIsActive = false;
	
	UPROPERTY(EditAnywhere)
	bool bAbsoluteScale;

	UPROPERTY(EditAnywhere)
	bool bAbsoluteRotation;

	UPROPERTY(EditAnywhere)
	bool bAbsoluteLocation;
};
