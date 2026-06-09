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
	virtual FVector CalcAttackDir(const FVector& forwardVector)const;

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

protected:

	/**	オーナー */
	UPROPERTY()
	TObjectPtr<AActor> OwnerActor;
	
	/** アタッチ用のルート（位置・回転管理） */
	UPROPERTY()
	TObjectPtr<USceneComponent> RootComponent;

	/**	攻撃可能フラグ */
	UPROPERTY()
	bool bCanExecute = true;

	/**	攻撃有効フラグ */
	UPROPERTY()
	bool bIsActive = false;

	/**
	 * @brief 攻撃時の位置・回転・スケールのオフセット値
	 */
	UPROPERTY(EditAnywhere)
	FTransform OffsetTransform = FTransform();
};
