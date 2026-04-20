#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "EnemySpawnPatternBase.generated.h"

// どこに敵を出すのかを決める基底クラス
// このクラスはインタフェース的な役割で
// 具体的な座標計算は派生クラスで行う

/// <summary>
/// 出現パラメータ
/// </summary>
USTRUCT(BlueprintType)
struct FSpawnPosParams
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
	/// 360度（degree）
	/// </summary>
	UPROPERTY(EditAnywhere)
	float kFullCircleDeg = 360.0f;
};

// UCLASS(Abstract)にすることで
// このクラス単体をBPやエディタ上で誤って使用できないようにしている
UCLASS(Abstract, EditInlineNew)
class PROJECTNULL_API UEnemySpawnPatternBase : public UObject
{
	GENERATED_BODY()

public:

	// BlueprintNativeEvent : 
	// C++ でデフォルト実装（_Implementation）を持ちつつ
	// BP側でオーバーライドもできる関数を宣言するマクロ

	// 実際に呼び出す側は GenerateSpawnTransforms()を呼ぶ
	// UEが自動的に「BPでオーバーライドされているか」を判定し
	// されていればBP版を、されていなければ_Implementationを呼ぶ
	// ※1
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	TArray<FVector> GenerateSpawnTransforms(
		int32 SpawnCount,
		const FVector& OriginVector) const;

	/// <summary>
	/// プレイヤー周囲のリング状スポーン座標を計算
	/// <param name="Center">中心座標</param>
	/// <returns>ランダム座標結果</returns>
	FVector CalculateEnemySpawnPointInRing(const FVector& Center) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Spawn")
	FSpawnPosParams SpawnPosParams;

};

// ※1
// BlueprintNativeEventはUHTが本体を自動生成する
// そのため、cppで同じ関数を宣言すると、2重定義になる
// cppを書く時には、_Implementationを添えて記述