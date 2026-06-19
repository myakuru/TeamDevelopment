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
	 */
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );
	
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
	 * @param forwardVector 基準となる前方ベクトル(オーナーの前方ベクトル)
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


	/* Begin ProtectedMenber Setters */
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
	 * @param bInIsActive trueで有効
	 */
	void SetIsActive(const bool InbIsActive) { bIsActive = InbIsActive; }
	/* End ProtectedMenber Setters */


	/* Begin ProtectedMenber Getters */
	/**
	 * @brief クラスのオーナーを取得
	 * @return クラスのオーナー
	 */
	TObjectPtr<AActor> GetOwnerActor()const { return OwnerActor; }

	/**
	 * @brief ルートを取得
	 * @return ルートコンポーネント
	 */
	inline USceneComponent* GetRootComponent()const { return RootComponent; }

	/**
	 * @brief 攻撃を当てたアクターリストを取得
	 * @return 攻撃が当たったアクターのリスト
	 */
	TSet<TWeakObjectPtr<AActor>> GetHitActors()const { return HitActors; }

	/**
	 * @brief オフセットトランスフォームを取得
	 * @return オフセットトランスフォーム
	 */
	FTransform GetOffsetTransform()const { return OffsetTransform; }

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
	/* End ProtectedMenber Getters */

	inline bool AbsoluteRotation() const { return bAbsoluteRotation;}


private:

	/**	オーナー */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
	
	/** アタッチ用のルート（位置・回転管理） */
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComponent;

	/** HITしたアクターリスト */
	UPROPERTY()
	TSet<TWeakObjectPtr<AActor>> HitActors;

	/**	攻撃可能フラグ */
	UPROPERTY()
	bool bCanExecute = true;

	/**	攻撃有効フラグ */
	UPROPERTY()
	bool bIsActive = false;

	/** 攻撃時の位置・回転・スケールのオフセット値 */
	UPROPERTY(EditAnywhere)
	FTransform OffsetTransform = FTransform();

	UPROPERTY(EditAnywhere)
	bool bAbsoluteScale;

	UPROPERTY(EditAnywhere)
	bool bAbsoluteRotation;

	UPROPERTY(EditAnywhere)
	bool bAbsoluteLocation;
};
