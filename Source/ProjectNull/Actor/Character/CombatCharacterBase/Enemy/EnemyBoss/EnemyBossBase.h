
#pragma once

#include "CoreMinimal.h"
#include "../../CombatCharacterBase.h"
#include "EnemyBossBase.generated.h"

class UPawnSensingComponent;
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

	FVector ActorLocation;

public:	
	virtual void Tick(float DeltaTime) override;

public:

	/**
	* プレイヤーを視界にとらえるためのコンポーネント
	* 視野角・視認距離はコンストラクタで設定
	 */
	UPROPERTY(VisibleAnywhere, Category = "AI")
	TObjectPtr<UPawnSensingComponent> PawnSensingComp;
	
	/*
	* PawnSensingがプレイヤーを見つけたときに呼ばれるコールバック
	* 検知したPawnをAIController経由でBlackboardのPlayerキーに渡す
	*/
	UFUNCTION()
	void OnSeePlayer(APawn* Pawn);

};
