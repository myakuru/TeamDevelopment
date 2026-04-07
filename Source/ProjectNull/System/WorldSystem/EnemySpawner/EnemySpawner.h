#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawner.generated.h"

/// <summary>
/// 360度（degree）
/// メモ：定数置く場所決まったら移動させる
/// </summary>
constexpr float kFullCircleDeg = 360.0f;

/// <summary>
/// 出現パラメータ
/// </summary>
USTRUCT(BlueprintType)
struct FSpawnParams
{
	GENERATED_BODY()

public:
	
	/// <summary>
	/// 最大出現半径
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnMaxRadius = 1000.0f;

	/// <summary>
	/// 最小出現半径
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnMinRadius = 100.0f;

	/// <summary>
	/// 出現間隔（時間）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnInterval = 2.0f;

	/// <summary>
	/// 地面として許容する法線Zの最小値
	/// </summary>
	UPROPERTY(EditAnywhere)
	float MinGroundNormalZ = 0.7f;

	/// <summary>
	/// レイ開始位置の高さ（上方向）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float RayStartHeight = 2000.0f;

	/// <summary>
	/// レイ終了位置の深さ（下方向）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float RayEndDepth = 2000.0f;

	/// <summary>
	/// 出現座標Zのオフセット
	/// </summary>
	UPROPERTY(EditAnywhere)
	float SpawnOffsetZ = 150.0f;

};

//　敵（一般兵/雑魚）の中間基底クラス
class AEnemyGruntBase;

/// <summary>
/// 敵の出現管理クラス
/// </summary>
UCLASS()
class PROJECTNULL_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawner();

protected:
	virtual void BeginPlay() override;

public:	

	/// <summary>
	/// 敵出現制御処理
	/// </summary>
	UFUNCTION(BlueprintCallable, Category = "EnemySpawner")
	void SpawnEnemy();

private:

	/// <summary>
	/// プレイヤー周囲のリング状スポーン座標を計算
	/// <param name="Center">中心座標</param>
	/// <returns>ランダム座標結果</returns>
	FVector CalculateEnemySpawnPointInRing(const FVector& Center) const;

	/// <summary>
	/// 静的オブジェクトと交差しているか
	/// </summary>
	/// <param name="hitResult">ヒットした際の結果</param>
	/// <param name="SpawnLocationXY">出現座標XY</param>
	/// <returns>交差しているかどうか</returns>
	bool IsIntersectingStaticObjects(FHitResult& HitResult,FVector& SpawnLocationXY);

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyGruntBase> EnemyClass;

	/// <summary>
	///　出現パラメータ
	/// </summary>
	UPROPERTY(EditAnywhere)
	FSpawnParams SpawnParams;

	/// <summary>
	/// 敵を一定時間ごとにスポーンするタイマーID
	/// </summary>
	FTimerHandle SpawnTimerHandle;
};
