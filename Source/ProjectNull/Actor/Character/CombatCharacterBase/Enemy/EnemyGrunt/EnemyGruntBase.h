
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
};
