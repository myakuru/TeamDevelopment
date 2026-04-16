#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "EnemyAttackComponent.generated.h"

// 敵キャラクラス
class AEnemyBase;

// 攻撃基底クラス
class UAttackBase;

/// <summary>
/// 敵キャラ(ワールド内で操作キャラ目掛けて追尾するキャラクター)の攻撃コンポーネントクラス
/// </summary>
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

private:

	// クラスの持ち主のアドレス
	UPROPERTY()
	AEnemyBase* OwnerEnemy;

	// 敵の攻撃クラスの配列リスト
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> EnemyAttacks;
};
