#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ProjectNull/System/Combat/CombatTypes/CombatTypes.h"
#include "EnemyAttackComponent.generated.h"

/**
 * @brief 敵の基底クラス
 */
class AEnemyBase;

/**
 * @brief 攻撃の基底クラス
 */
class UAttackBase;

/**
 * @brief 敵の攻撃コンポーネント
 */
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UEnemyAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	

	UEnemyAttackComponent();

protected:

	virtual void BeginPlay() override;

public:	

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	virtual void SetOwnerEnemy(AEnemyBase* Enemy) { OwnerEnemy = Enemy; }

public:

	/**
	 * @brief 全ての攻撃終了
	 */
	void AllAtackDeactivate();

	/**
	 * @brief 全ての攻撃が終了しているか
	 * @return 終了しているならtrue
	 */
	bool IsAllAttackDeactivate();

	/**
	 * @brief 指定の攻撃が終了しているか
	 * @return 終了しているならtrue
	 */
	// 指定方法が未定の為今は未実装
	bool IsAttackDeactivate();

	/**
	 * @brief 攻撃有効化
	 * @param AttackIndex 有効化したいインデックス番号
	 */
	void AttackActive(EEnemyAttackType InAttackType);

	void TestActive();

	/**	ターゲットの座標をセット */
	void SetTargetLocation(const FVector& InTargetLocation) { TargetLocation = InTargetLocation; }
	
private:

	/**
	 * @brief 持ち主
	 */
	UPROPERTY()
	AEnemyBase* OwnerEnemy;

	/**
	 * @brief 攻撃オブジェクトリスト
	 * キー : EEnemyAttackType 攻撃タイプ
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TMap<EEnemyAttackType, TObjectPtr<UAttackBase>> EnemyAttacks;

	/**
	 * @brief それぞれの攻撃に対応したタイマーハンドル
	 */
	TMap<EEnemyAttackType,FTimerHandle> AttackTimerHandles;

	/**
	 * @brief ターゲットの座標
	 */
	UPROPERTY()
	FVector TargetLocation;
};
