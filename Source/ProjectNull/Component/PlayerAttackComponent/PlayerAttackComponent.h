#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PlayerAttackComponent.generated.h"


// プレイヤークラス
class APlayerBase;

// 攻撃基底クラス
class UAttackBase;

/// <summary>
/// プレイヤー（ワールド内の自身が操作するキャラクター）の攻撃コンポーネントクラス
/// </summary>
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECTNULL_API UPlayerAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UPlayerAttackComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// セッター
	inline void SetOwnerPlayer(APlayerBase* Player) { OwnerPlayer = Player; }

private:

	/// <summary>
	/// 攻撃クラスを作成し、リストへ追加
	/// </summary>
	/// <typeparam name="AttackType">攻撃クラスの種類</typeparam>
	/// <returns>作成した攻撃クラス</returns>
	template<typename AttackType>
	AttackType* AddAttack();

	/// <summary>
	/// リストから攻撃クラスを検索して取得
	/// </summary>
	/// <typeparam name="AttackType">攻撃クラスの種類</typeparam>
	/// <returns>検索して取得した攻撃クラス</returns>
	template<typename AttackType>
	AttackType* FindAttack();


	// プレイヤー（持ち主）のポインタ
	UPROPERTY()
	APlayerBase* OwnerPlayer;

	// プレイヤーの攻撃クラスの配列
	UPROPERTY(EditAnywhere, Instanced)
	TArray<UAttackBase*> PlayerAttacks;

};
