#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "AttackBase.generated.h"

// �G�Ǘ��N���X
class UEnemyManagerSubsystem;

// �v���C���[���N���X
class APlayerBase;

class USceneComponent;

/// <summary>
/// �U���̊��N���X
/// �v���C���[��G�̍U���R���|�[�l���g�N���X�Ŏg�p�����U���N���X�̊��N���X
/// </summary>
UCLASS()
class PROJECTNULL_API UAttackBase : public UObject
{
	GENERATED_BODY()

public:

	UAttackBase();

public:

	/**
	 * @brief 初期化
	 * @param Owner 
	 */
	virtual void Initialize(class AActor* Owner);

	/**
	 * @brief 有効化処理
	 */
	virtual void Execute() PURE_VIRTUAL(UAttackBase::Execute, );
	
	/**
	 * @brief 更新処理
	 * @param DeltaTime デルタタイム
	 */
	virtual void Update(float DeltaTime) { return; }

	/**
	 * @brief 当たり判定処理
	 */
	virtual void AttackJudge();

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
	AActor* OwnerActor;
	
	/** アタッチ用のルート（位置・回転管理） */
	UPROPERTY()
	USceneComponent* RootComponent;

	/**	攻撃可能フラグ */
	bool bCanExecute = true;

	/**	攻撃有効フラグ */
	bool bIsActive = false;

	FTransform Transform;
};
