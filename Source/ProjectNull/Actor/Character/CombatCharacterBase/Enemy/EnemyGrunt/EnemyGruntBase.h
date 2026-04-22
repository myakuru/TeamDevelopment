
#pragma once

#include "CoreMinimal.h"
#include "../EnemyBase.h"
#include "EnemyGruntBase.generated.h"

/// <summary>
/// 敵（一般兵/雑魚）の中間基底クラス
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemyGruntBase : public AEnemyBase
{
	GENERATED_BODY()

public:
	AEnemyGruntBase();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/// <summary>
	/// 敵更新メソッド
	/// </summary>
	/// <param name="Player">プレイヤー</param>
	/// <param name="DeltaTime">デルタタイム</param>
	virtual void OnUpdate(APawn* Player, float DeltaTime) override;

private:

	//// 他のActorと重なった際に呼ばれる関数
	//virtual void OnOverlap(
	//	UPrimitiveComponent* OverlappedComponent,	// 自分側のコリジョン
	//	AActor* OtherActor,							// 触れてきたアクタ
	//	UPrimitiveComponent* OtherComp,				// 触れてきた側のコンポーネント
	//	int32 OtherBodyIndex,						// ボディ番号（基本使わない）
	//	bool bFromSweep,							// Sweepかどうか
	//	const FHitResult& SweepResult				// 当たりの詳細判定
	//) override;

};
