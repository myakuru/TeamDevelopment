#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
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
	 * @brief 全ての攻撃が終了しているか
	 * @return 終了しているならtrue
	 */
	bool IsAllAttackDeactivate();

	/**
	 * @brief 指定の攻撃が終了しているか
	 * @return 終了しているならtrue
	 */
	// 指定方法が未定の為今は未実装
	//bool IsAttackDeactivate();

	void TestActive();

private:

	/**
	 * @brief 持ち主
	 */
	UPROPERTY()
	AEnemyBase* OwnerEnemy;

	/**
	 * @brief 攻撃オブジェクトリスト
	 */
	UPROPERTY(EditAnywhere, Instanced)
	TArray<TObjectPtr<UAttackBase>> EnemyAttacks;
};
